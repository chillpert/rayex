#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "api/Surface.hpp"
#include "api/image/Image.hpp"
#include "api/utility/Helpers.hpp"

namespace RENDERER_NAMESPACE
{
  /// A wrapper class for a Vulkan swapchain.
  /// @ingroup API
  class Swapchain
  {
  public:
    Swapchain( ) = default;

    /// @param surface A pointer to a rx::Surface object.
    /// @param renderPass The render pass to create the framebuffers.
    /// @param initialize If true, the swapchain object will be initialized right away without an additional call to init().
    Swapchain( Surface* surface, vk::RenderPass renderPass, bool initialize = true );

    /// Creates the swapchain, the swapchain images and their image views as well as their framebuffers.
    /// @param surface A pointer to a rx::Surface object.
    /// @param renderPass The render pass to create the framebuffers.
    void init( Surface* surface, vk::RenderPass renderPass );

    /// Destroys the swapchain.
    void destroy( );

    /// @return Returns the current swapchain image index.
    inline uint32_t getCurrentImageIndex( ) const { return m_currentImageIndex; }

    /// @return Returns the swapchain images' extent.
    inline const vk::Extent2D getExtent( ) const { return m_extent; }

    /// @return Returns the swapchain images' image aspect.
    inline const vk::ImageAspectFlags getImageAspect( ) const { return m_imageAspect; }

    /// Returns the swapchain image at a given index.
    /// @param index The index of the swapchain image.
    /// @return The swapchain image.
    inline const vk::Image getImage( size_t index ) const { return m_images[index]; }

    /// @return Returns a vector containing all swapchain images.
    inline const std::vector<vk::Image>& getImages( ) const { return m_images; }

    /// @return Returns a vector containing all swapchain image views.
    /// @todo Returning by reference will result in size 0.
    inline const std::vector<vk::ImageView> getImageViews( ) const { return vk::Helper::unpack<vk::ImageView>( m_imageViews ); } 

    /// Used to set the desired image aspect flags.
    void setImageAspect( vk::ImageAspectFlags flags );

    /// Used to transition from one layout to another.
    /// @param oldLayout The swapchain images' current image layout.
    /// @param newLayout The target image layout.
    void setImageLayout( vk::ImageLayout oldLayout, vk::ImageLayout newLayout );

    /// Retrieves the next swapchain image.
    /// @param semaphore A semaphore to signal.
    /// @param fence A fence to signal.
    void acquireNextImage( vk::Semaphore semaphore, vk::Fence fence );

  private:
    /// Initializes the swapchain images.
    /// @param minImageCount The minimum amount of images in the swapchain.
    /// @param surfaceFormat The surface's format.
    void initImages( uint32_t minImageCount, vk::Format surfaceFormat );
    
    /// Initializes the depth image for the depth attachment.
    void initDepthImage( );

    /// Initializes the swapchain framebuffers.
    /// @param renderPass The render pass to create the framebuffers.
    void initFramebuffers( vk::RenderPass renderPass );

    vk::UniqueSwapchainKHR m_swapchain; ///< The Vulkan swapchain object with a unique handle.

    vk::Extent2D m_extent; ///< The swapchain images' extent.
    vk::ImageAspectFlags m_imageAspect = vk::ImageAspectFlagBits::eColor; ///< The swapchain image's image aspect.

    std::vector<vk::Image> m_images; ///< The swapchain images.
    std::vector<vk::UniqueImageView> m_imageViews; ///< The swapchain images' image views with a unique handle.

    std::vector<vk::UniqueFramebuffer> m_framebuffers; ///< The swapchain images' framebuffers with a unique handle.

    Image m_depthImage; ///< The depth image.
    vk::UniqueImageView m_depthImageView; ///< The depth image's image views with a unique handle. 

    uint32_t m_currentImageIndex; ///< The current swapchain image index.
  };

  /// Retrieves the depth format supported by a given physical device.
  /// @param physicalDevice The physical device to check.
  /// @return Returns the supported depth format.
  vk::Format getSupportedDepthFormat( vk::PhysicalDevice physicalDevice );
}

#endif // SWAPCHAIN_HPP