#include "api/Swapchain.hpp"
#include "api/misc/Components.hpp"
#include "api/utility/Initializers.hpp"

namespace RENDERER_NAMESPACE
{
  bool Swapchain::init( Surface* surface, vk::RenderPass renderPass )
  {
    surface->checkSettingSupport( );

    auto surfaceCapabilities = surface->getCapabilities( );

    vk::SwapchainCreateInfoKHR createInfo;
    createInfo.surface = g_surface;

    // Add another image so that the application does not have to wait for the driver before another image can be acquired.
    uint32_t minImageCount = surfaceCapabilities.minImageCount + 1;

    if ( surfaceCapabilities.maxImageCount == 0 )
    {
      RX_ERROR( "The surface does not support any images for a swap chain." );
      return false;
    }

    // If the preferred image count is exceeding the supported amount then use the maximum amount of images supported by the surface.
    if ( minImageCount > surfaceCapabilities.maxImageCount )
      minImageCount = surfaceCapabilities.maxImageCount;

    createInfo.minImageCount = minImageCount;
    createInfo.imageFormat = surface->getFormat( );
    createInfo.imageColorSpace = surface->getColorSpace( );
    createInfo.preTransform = surfaceCapabilities.currentTransform;

    // Prefer opaque bit over any other composite alpha value.
    createInfo.compositeAlpha = surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eOpaque ? vk::CompositeAlphaFlagBitsKHR::eOpaque :
                                surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied :
                                surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied :
                                vk::CompositeAlphaFlagBitsKHR::eInherit;

    // Handle the swap chain image extent.
    if ( surfaceCapabilities.currentExtent.width != UINT32_MAX )
    {
      // The surface size will be determined by the extent of a swapchain targeting the surface.
      this->extent = surfaceCapabilities.currentExtent;
    }
    else
    {
      // Clamp width and height.
      this->extent = g_window->getExtent( );

      uint32_t width_t = this->extent.width;
      if ( surfaceCapabilities.maxImageExtent.width < this->extent.width )
        width_t = surfaceCapabilities.maxImageExtent.width;

      uint32_t height_t = this->extent.height;
      if ( surfaceCapabilities.maxImageExtent.height < this->extent.height )
        height_t = surfaceCapabilities.maxImageExtent.height;

      this->extent.width = width_t;
      if ( surfaceCapabilities.minImageExtent.width > width_t )
        this->extent.width = surfaceCapabilities.minImageExtent.width;

      this->extent.height = height_t;
      if ( surfaceCapabilities.minImageExtent.height > height_t )
        this->extent.height = surfaceCapabilities.minImageExtent.height;
    }

    createInfo.imageExtent = this->extent;

    if ( surfaceCapabilities.maxImageArrayLayers < 1 )
    {
      RX_ERROR( "The surface does not support a single array layer." );
      return false;
    }

    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst;

    std::vector<uint32_t> queueFamilyIndices = { g_graphicsFamilyIndex };

    if ( queueFamilyIndices.size( ) > 1 )
    {
      createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
      createInfo.queueFamilyIndexCount = static_cast<uint32_t>( queueFamilyIndices.size( ) );
      createInfo.pQueueFamilyIndices = queueFamilyIndices.data( );
    }
    else
      createInfo.imageSharingMode = vk::SharingMode::eExclusive;

    createInfo.presentMode = surface->getPresentMode( );

    this->swapchain = g_device.createSwapchainKHRUnique( createInfo );
    if ( !this->swapchain )
    {
      RX_ERROR( "Failed to create swapchain." );
      return false;
    }

    g_swapchain = this->swapchain.get( );

    bool result = initImages( minImageCount, surface->getFormat( ) );
    RX_ASSERT_INIT( result );

    result = initDepthImage( );
    RX_ASSERT_INIT( result );
    
    result = initFramebuffers( renderPass );
    return result;
  }

  void Swapchain::destroy( )
  {
    if ( this->swapchain )
    {
      g_device.destroySwapchainKHR( this->swapchain.get( ) );
      this->swapchain.get( ) = nullptr;
    }
  }

  void Swapchain::setImageAspect( vk::ImageAspectFlags flags )
  {
    this->imageAspect = flags;
  }

  void Swapchain::setImageLayout( vk::ImageLayout oldLayout, vk::ImageLayout newLayout )
  {
    for ( const auto& image : this->images )
    {
      vk::Helper::transitionImageLayout( image, oldLayout, newLayout );
    }
  }

  void Swapchain::acquireNextImage( vk::Semaphore semaphore, vk::Fence fence )
  {
    vk::Result result = g_device.acquireNextImageKHR( this->swapchain.get( ), UINT64_MAX, semaphore, fence, &this->currentImageIndex );
    RX_ASSERT( ( result == vk::Result::eSuccess ), "Failed to acquire next swapchain image." );
  }

  bool Swapchain::initImages( uint32_t minImageCount, vk::Format surfaceFormat )
  {
    // Retrieve the actual swapchain images. This sets them up automatically.
    this->images = g_device.getSwapchainImagesKHR( this->swapchain.get( ) );
    if ( this->images.size( ) < minImageCount )
    {
      RX_ERROR( "Failed to get swapchain images." );
      return false;
    }

    g_swapchainImageCount = static_cast<uint32_t>( this->images.size( ) );

    // Create image views for swapchain images.
    this->imageViews.resize( this->images.size( ) );
    for ( size_t i = 0; i < this->imageViews.size( ); ++i )
      this->imageViews[i] = vk::Initializer::initImageViewUnique( this->images[i], surfaceFormat, this->imageAspect );
    
    return true;
  }

  bool Swapchain::initDepthImage( )
  {
    // Depth image for depth buffering
    vk::Format depthFormat = getSupportedDepthFormat( g_physicalDevice );

    auto imageCreateInfo = vk::Helper::getImageCreateInfo( vk::Extent3D( this->extent.width, this->extent.height, 1 ) );
    imageCreateInfo.format = depthFormat;
    imageCreateInfo.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;

    bool result = this->depthImage.init( imageCreateInfo );
    RX_ASSERT_INIT( result );

    // Image view for depth image
    this->depthImageView = vk::Initializer::initImageViewUnique( this->depthImage.get( ), depthFormat, vk::ImageAspectFlagBits::eDepth );
    return true;
  }

  bool Swapchain::initFramebuffers( vk::RenderPass renderPass )
  {
    this->framebuffers.resize( this->imageViews.size( ) );
    for ( size_t i = 0; i < this->framebuffers.size( ); ++i )
      this->framebuffers[i] = vk::Initializer::initFramebufferUnique( { this->imageViews[i].get( ), this->depthImageView.get( ) }, renderPass, this->extent );
  
    return true;
  }

  vk::Format getSupportedDepthFormat( vk::PhysicalDevice physicalDevice )
  {
    std::vector<vk::Format> candidates { vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint };
    return Image::findSupportedFormat( physicalDevice, candidates, vk::FormatFeatureFlagBits::eDepthStencilAttachment, vk::ImageTiling::eOptimal );
  }
}