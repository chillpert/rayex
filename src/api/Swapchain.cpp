#include "Swapchain.hpp"
#include "Components.hpp"

namespace rx
{
  Swapchain::Swapchain( Surface* surface, bool initialize )
  {
    if ( initialize )
      init( surface );
  }

  void Swapchain::init( Surface* surface )
  {
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
    createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

    std::vector<uint32_t> queueFamilyIndices =
    {
      g_graphicsFamilyIndex
    };

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

    // Get swapchain images.
    m_images = g_device.getSwapchainImagesKHR( m_swapchain.get( ) );
    if ( m_images.size( ) < minImageCount )
      RX_ERROR( "Failed to get swapchain images." );

    g_swapchainImageCount = static_cast<uint32_t>( m_images.size( ) );
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

  void Swapchain::acquireNextImage( vk::Semaphore semaphore, vk::Fence fence, uint32_t* imageIndex )
  {
    g_device.acquireNextImageKHR( m_swapchain.get( ), UINT64_MAX, semaphore, fence, imageIndex );
  }

  vk::Format getSupportedDepthFormat( vk::PhysicalDevice physicalDevice )
  {
    std::vector<vk::Format> candidates { vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint };
    return Image::findSupportedFormat( physicalDevice, candidates, vk::FormatFeatureFlagBits::eDepthStencilAttachment, vk::ImageTiling::eOptimal );
  }
}