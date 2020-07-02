#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "Surface.hpp"
#include "Image.hpp"

namespace rx
{
  class Swapchain
  {
  public:
    Swapchain( ) = default;
    Swapchain( Surface* surface, bool initialize = true );

    // Creates the swapchain.
    // @param surface - Pointer to an rx::Surface object.
    // @param queueFamilyIndices - All queue family indices which shall be accessed by the swapchain.
    void init( Surface* surface );
    void destroy( );

    inline const vk::Extent2D getExtent( ) const { return m_extent; }
    inline const vk::Image getImage( size_t index ) const { return m_images[index]; }
    inline const std::vector<vk::Image>& getImages( ) const { return m_images; }
    inline const vk::ImageAspectFlags getImageAspect( ) const { return m_imageAspect; }

    void setImageAspect( vk::ImageAspectFlags flags );

    void acquireNextImage( vk::Semaphore semaphore, vk::Fence fence, uint32_t* imageIndex );

  private:
    vk::UniqueSwapchainKHR m_swapchain;

    vk::Extent2D m_extent;
    std::vector<vk::Image> m_images;
    vk::ImageAspectFlags m_imageAspect = vk::ImageAspectFlagBits::eColor;
  };

  vk::Format getSupportedDepthFormat( vk::PhysicalDevice physicalDevice );
}

#endif // SWAPCHAIN_HPP