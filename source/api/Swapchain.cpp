#include "api/Swapchain.hpp"

#include "api/Components.hpp"
#include "api/utility/Initializers.hpp"

namespace RAYEX_NAMESPACE
{
  void Swapchain::init( Surface* surface, vk::RenderPass renderPass )
  {
    surface->assessSettings( );

    auto surfaceCapabilities = surface->getCapabilities( );

    vk::SwapchainCreateInfoKHR createInfo;
    createInfo.surface = g_surface;

    // Add another image so that the application does not have to wait for the driver before another image can be acquired.
    uint32_t minImageCount = surfaceCapabilities.minImageCount + 1;

    if ( surfaceCapabilities.maxImageCount == 0 )
    {
      RX_FATAL( "The surface does not support any images for a swap chain." );
    }

    // If the preferred image count is exceeding the supported amount then use the maximum amount of images supported by the surface.
    if ( minImageCount > surfaceCapabilities.maxImageCount )
    {
      minImageCount = surfaceCapabilities.maxImageCount;
    }

    createInfo.minImageCount   = minImageCount;
    createInfo.imageFormat     = surface->getFormat( );
    createInfo.imageColorSpace = surface->getColorSpace( );
    createInfo.preTransform    = surfaceCapabilities.currentTransform;

    // Prefer opaque bit over any other composite alpha value.
    createInfo.compositeAlpha = surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eOpaque ? vk::CompositeAlphaFlagBitsKHR::eOpaque :
                                                                                                                       surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied :
                                                                                                                                                                                                                     surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied :
                                                                                                                                                                                                                                                                                                                    vk::CompositeAlphaFlagBitsKHR::eInherit;

    // Handle the swap chain image extent.
    if ( surfaceCapabilities.currentExtent.width != UINT32_MAX )
    {
      // The surface size will be determined by the extent of a swapchain targeting the surface.
      _extent = surfaceCapabilities.currentExtent;
    }
    else
    {
      // Clamp width and height.
      _extent = g_window->getExtent( );

      uint32_t width_t = _extent.width;
      if ( surfaceCapabilities.maxImageExtent.width < _extent.width )
      {
        width_t = surfaceCapabilities.maxImageExtent.width;
      }

      uint32_t height_t = _extent.height;
      if ( surfaceCapabilities.maxImageExtent.height < _extent.height )
      {
        height_t = surfaceCapabilities.maxImageExtent.height;
      }

      _extent.width = width_t;
      if ( surfaceCapabilities.minImageExtent.width > width_t )
      {
        _extent.width = surfaceCapabilities.minImageExtent.width;
      }

      _extent.height = height_t;
      if ( surfaceCapabilities.minImageExtent.height > height_t )
      {
        _extent.height = surfaceCapabilities.minImageExtent.height;
      }
    }

    createInfo.imageExtent = _extent;

    if ( surfaceCapabilities.maxImageArrayLayers < 1 )
    {
      RX_FATAL( "The surface does not support a single array layer." );
    }

    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage       = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst;

    std::vector<uint32_t> queueFamilyIndices = { g_graphicsFamilyIndex };

    if ( queueFamilyIndices.size( ) > 1 )
    {
      createInfo.imageSharingMode      = vk::SharingMode::eConcurrent;
      createInfo.queueFamilyIndexCount = static_cast<uint32_t>( queueFamilyIndices.size( ) );
      createInfo.pQueueFamilyIndices   = queueFamilyIndices.data( );
    }
    else
    {
      createInfo.imageSharingMode = vk::SharingMode::eExclusive;
    }

    createInfo.presentMode = surface->getPresentMode( );

    _swapchain = g_device.createSwapchainKHRUnique( createInfo );
    RX_ASSERT( _swapchain, "Failed to create swapchain" );
    g_swapchain = _swapchain.get( );

    initImages( minImageCount, surface->getFormat( ) );
    initDepthImage( );
    initFramebuffers( renderPass );
  }

  void Swapchain::destroy( )
  {
    if ( _swapchain )
    {
      g_device.destroySwapchainKHR( _swapchain.get( ) );
      _swapchain.get( ) = nullptr;
    }
  }

  void Swapchain::setImageAspect( vk::ImageAspectFlags flags )
  {
    _imageAspect = flags;
  }

  void Swapchain::setImageLayout( vk::ImageLayout oldLayout, vk::ImageLayout newLayout )
  {
    for ( const auto& image : _images )
    {
      vk::Helper::transitionImageLayout( image, oldLayout, newLayout );
    }
  }

  void Swapchain::acquireNextImage( vk::Semaphore semaphore, vk::Fence fence )
  {
    vk::Result result = g_device.acquireNextImageKHR( _swapchain.get( ), UINT64_MAX, semaphore, fence, &_currentImageIndex );
    RX_ASSERT( ( result == vk::Result::eSuccess ), "Failed to acquire next swapchain image." );
  }

  void Swapchain::initImages( uint32_t minImageCount, vk::Format surfaceFormat )
  {
    // Retrieve the actual swapchain images. This sets them up automatically.
    _images = g_device.getSwapchainImagesKHR( _swapchain.get( ) );
    if ( _images.size( ) < minImageCount )
    {
      RX_FATAL( "Failed to get swapchain images." );
    }

    g_swapchainImageCount = static_cast<uint32_t>( _images.size( ) );

    // Create image views for swapchain images.
    _imageViews.resize( _images.size( ) );
    for ( size_t i = 0; i < _imageViews.size( ); ++i )
    {
      _imageViews[i] = vk::Initializer::initImageViewUnique( _images[i], surfaceFormat, _imageAspect );
    }
  }

  void Swapchain::initDepthImage( )
  {
    // Depth image for depth buffering
    vk::Format depthFormat = getSupportedDepthFormat( g_physicalDevice );

    auto imageCreateInfo   = vk::Helper::getImageCreateInfo( vk::Extent3D( _extent.width, _extent.height, 1 ) );
    imageCreateInfo.format = depthFormat;
    imageCreateInfo.usage  = vk::ImageUsageFlagBits::eDepthStencilAttachment;

    _depthImage.init( imageCreateInfo );

    // Image view for depth image
    _depthImageView = vk::Initializer::initImageViewUnique( _depthImage.get( ), depthFormat, vk::ImageAspectFlagBits::eDepth );
  }

  void Swapchain::initFramebuffers( vk::RenderPass renderPass )
  {
    _framebuffers.resize( _imageViews.size( ) );
    for ( size_t i = 0; i < _framebuffers.size( ); ++i )
    {
      _framebuffers[i] = vk::Initializer::initFramebufferUnique( { _imageViews[i].get( ), _depthImageView.get( ) }, renderPass, _extent );
    }
  }

  auto getSupportedDepthFormat( vk::PhysicalDevice physicalDevice ) -> vk::Format
  {
    std::vector<vk::Format> candidates { vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint };
    return vk::Helper::findSupportedImageFormat( physicalDevice, candidates, vk::FormatFeatureFlagBits::eDepthStencilAttachment, vk::ImageTiling::eOptimal );
  }
} // namespace RAYEX_NAMESPACE
