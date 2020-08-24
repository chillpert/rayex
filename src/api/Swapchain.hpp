#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "Surface.hpp"
#include "Image.hpp"
#include "Helpers.hpp"

namespace rx
{
  class Swapchain
  {
  public:
    Swapchain( ) = default;
    Swapchain( Surface* surface, vk::RenderPass renderPass, bool initialize = true );

    // Creates the swapchain.
    // @param surface - Pointer to an rx::Surface object.
    void init( Surface* surface, vk::RenderPass renderPass );
    void destroy( );

    inline uint32_t getCurrentImageIndex( ) const { return m_currentImageIndex; }

    inline const vk::Extent2D getExtent( ) const { return m_extent; }
    inline const vk::ImageAspectFlags getImageAspect( ) const { return m_imageAspect; }

    inline const vk::Image getImage( size_t index ) const { return m_images[index]; }

    inline const std::vector<vk::Image>& getImages( ) const { return m_images; }
    inline const std::vector<vk::ImageView> getImageViews( ) const { return vk::Helper::unpack( m_imageViews ); } // Returning by reference will result in size 0.

    void setImageAspect( vk::ImageAspectFlags flags );
    void setImageLayout( vk::ImageLayout oldLayout, vk::ImageLayout newLayout );

    void acquireNextImage( vk::Semaphore semaphore, vk::Fence fence );

  private:
    void initImages( uint32_t minImageCount, vk::Format surfaceFormat );
    void initDepthImage( );
    void initFramebuffers( vk::RenderPass renderPass );

    vk::UniqueSwapchainKHR m_swapchain;

    vk::Extent2D m_extent;
    vk::ImageAspectFlags m_imageAspect = vk::ImageAspectFlagBits::eColor;

    std::vector<vk::Image> m_images;
    std::vector<vk::UniqueImageView> m_imageViews;

    std::vector<vk::UniqueFramebuffer> m_framebuffers;

    Image m_depthImage;
    vk::UniqueImageView m_depthImageView;

    uint32_t m_currentImageIndex;
  };

  vk::Format getSupportedDepthFormat( vk::PhysicalDevice physicalDevice );
}

#endif // SWAPCHAIN_HPP