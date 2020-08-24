#include "Swapchain.hpp"
#include "Components.hpp"
#include "Initializers.hpp"

namespace rx
{
  Swapchain::Swapchain( Surface* surface, vk::RenderPass renderPass, bool initialize )
  {
    if ( initialize )
      init( surface, renderPass );
  }

  void Swapchain::init( Surface* surface, vk::RenderPass renderPass )
  {
    surface->checkSettingSupport( );

    auto surfaceCapabilities = surface->getCapabilities( );

    vk::SwapchainCreateInfoKHR createInfo;
    createInfo.surface = g_surface;

    // Add another image so that the application does not have to wait for the driver before another image can be acquired.
    uint32_t minImageCount = surfaceCapabilities.minImageCount + 1;

    if ( surfaceCapabilities.maxImageCount == 0 )
      RX_ERROR( "The surface does not support any images for a swap chain." );

    // If the preferred image count is exceeding the supported amount then use the maximum amount of images supported by the surface.
    if ( minImageCount > surfaceCapabilities.maxImageCount )
      minImageCount = surfaceCapabilities.maxImageCount;

    createInfo.minImageCount = minImageCount;
    createInfo.imageFormat = surface->getFormat( );
    createInfo.imageColorSpace = surface->getColorSpace( );
    createInfo.preTransform = surfaceCapabilities.currentTransform;

    // Prefer opaque bit over any other composite alpha value.
    createInfo.compositeAlpha =
      surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eOpaque ? vk::CompositeAlphaFlagBitsKHR::eOpaque :
      surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied :
      surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied :
      vk::CompositeAlphaFlagBitsKHR::eInherit;

    // Handle the swap chain image extent.
    if ( surfaceCapabilities.currentExtent.width != UINT32_MAX )
    {
      // The surface size will be determined by the extent of a swapchain targeting the surface.
      m_extent = surfaceCapabilities.currentExtent;
    }
    else
    {
      // Clamp width and height.
      m_extent = g_window->getExtent( );

      uint32_t width_t = m_extent.width;
      if ( surfaceCapabilities.maxImageExtent.width < m_extent.width )
        width_t = surfaceCapabilities.maxImageExtent.width;

      uint32_t height_t = m_extent.height;
      if ( surfaceCapabilities.maxImageExtent.height < m_extent.height )
        height_t = surfaceCapabilities.maxImageExtent.height;

      m_extent.width = width_t;
      if ( surfaceCapabilities.minImageExtent.width > width_t )
        m_extent.width = surfaceCapabilities.minImageExtent.width;

      m_extent.height = height_t;
      if ( surfaceCapabilities.minImageExtent.height > height_t )
        m_extent.height = surfaceCapabilities.minImageExtent.height;
    }

    createInfo.imageExtent = m_extent;

    if ( surfaceCapabilities.maxImageArrayLayers < 1 )
      RX_ERROR( "The surface does not support a single array layer." );

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

    m_swapchain = g_device.createSwapchainKHRUnique( createInfo );
    if ( !m_swapchain )
      RX_ERROR( "Failed to create swapchain." );

    g_swapchain = m_swapchain.get( );

    initImages( minImageCount, surface->getFormat( ) );
    initDepthImage( );
    initFramebuffers( renderPass );
  }

  void Swapchain::destroy( )
  {
    if ( m_swapchain )
    {
      g_device.destroySwapchainKHR( m_swapchain.get( ) );
      m_swapchain.get( ) = nullptr;
    }
  }

  void Swapchain::setImageAspect( vk::ImageAspectFlags flags )
  {
    m_imageAspect = flags;
  }

  void Swapchain::setImageLayout( vk::ImageLayout oldLayout, vk::ImageLayout newLayout )
  {
    for ( const auto& image : m_images )
    {
      vk::Helper::transitionImageLayout( image, oldLayout, newLayout );
    }
  }

  void Swapchain::acquireNextImage( vk::Semaphore semaphore, vk::Fence fence, uint32_t* imageIndex )
  {
    g_device.acquireNextImageKHR( m_swapchain.get( ), UINT64_MAX, semaphore, fence, imageIndex );
  }

  void Swapchain::initImages( uint32_t minImageCount, vk::Format surfaceFormat )
  {
    // Retrieve the actual swapchain images. This sets them up automatically.
    m_images = g_device.getSwapchainImagesKHR( m_swapchain.get( ) );
    RX_ASSERT( m_images.size( ) >= minImageCount, "Failed to get swapchain images." );

    g_swapchainImageCount = static_cast<uint32_t>( m_images.size( ) );

    // Create image views for swapchain images.
    m_imageViews.resize( m_images.size( ) );
    for ( size_t i = 0; i < m_imageViews.size( ); ++i )
    {
      m_imageViews[i] = vk::Initializer::createImageViewUnique( m_images[i], surfaceFormat, m_imageAspect );
    }
  }

  void Swapchain::initDepthImage( )
  {
    // Depth image for depth buffering
    vk::Format depthFormat = getSupportedDepthFormat( g_physicalDevice );

    auto imageCreateInfo = vk::Helper::getImageCreateInfo( vk::Extent3D( m_extent.width, m_extent.height, 1 ) );
    imageCreateInfo.format = depthFormat;
    imageCreateInfo.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;

    m_depthImage.init( imageCreateInfo );

    // Image view for depth image
    m_depthImageView = vk::Initializer::createImageViewUnique( m_depthImage.get( ), depthFormat, vk::ImageAspectFlagBits::eDepth );
  }

  void Swapchain::initFramebuffers( vk::RenderPass renderPass )
  {
    m_framebuffers.resize( m_imageViews.size( ) );
    for ( size_t i = 0; i < m_framebuffers.size( ); ++i )
    {
      m_framebuffers[i] = vk::Initializer::createFramebufferUnique( { m_imageViews[i].get( ), m_depthImageView.get( ) }, renderPass, m_extent );
    }
  }

  vk::Format getSupportedDepthFormat( vk::PhysicalDevice physicalDevice )
  {
    std::vector<vk::Format> candidates { vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint };
    return Image::findSupportedFormat( physicalDevice, candidates, vk::FormatFeatureFlagBits::eDepthStencilAttachment, vk::ImageTiling::eOptimal );
  }
}