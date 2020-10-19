#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "api/Surface.hpp"
#include "api/image/Image.hpp"
#include "api/utility/Helpers.hpp"

namespace RAYEX_NAMESPACE
{
  /// A wrapper class for a Vulkan swapchain.
  /// @ingroup API
  class Swapchain
  {
  public:
    /// Creates the swapchain, the swapchain images and their image views as well as their framebuffers.
    /// @param surface A pointer to a RAYEX_NAMESPACE::Surface object.
    /// @param renderPass The render pass to create the framebuffers.
    void init( Surface* surface, vk::RenderPass renderPass );

    /// Destroys the swapchain.
    void destroy( );

    /// @return Returns the swapchain framebuffer at a given index.
    [[nodiscard]] auto getFramebuffer( uint32_t index ) const -> const vk::Framebuffer& { return this->framebuffers[index].get( ); }

    /// @return Returns the current swapchain image index.
    [[nodiscard]] auto getCurrentImageIndex( ) const -> uint32_t { return this->currentImageIndex; }

    /// @return Returns the swapchain images' extent.
    [[nodiscard]] auto getExtent( ) const -> vk::Extent2D { return this->extent; }

    /// @return Returns the swapchain images' image aspect.
    [[nodiscard]] auto getImageAspect( ) const -> vk::ImageAspectFlags { return this->imageAspect; }

    /// Returns the swapchain image at a given index.
    /// @param index The index of the swapchain image.
    /// @return The swapchain image.
    [[nodiscard]] auto getImage( size_t index ) const -> vk::Image { return this->images[index]; }

    /// @return Returns a vector containing all swapchain images.
    [[nodiscard]] auto getImages( ) const -> const std::vector<vk::Image>& { return this->images; }

    /// @return Returns a vector containing all swapchain image views.
    /// @todo Returning by reference will result in size 0.
    [[nodiscard]] auto getImageViews( ) const -> std::vector<vk::ImageView> { return vk::Helper::unpack<vk::ImageView>( this->imageViews ); }

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

    vk::UniqueSwapchainKHR swapchain; ///< The Vulkan swapchain object with a unique handle.

    vk::Extent2D extent;                                                ///< The swapchain images' extent.
    vk::ImageAspectFlags imageAspect = vk::ImageAspectFlagBits::eColor; ///< The swapchain image's image aspect.

    std::vector<vk::Image> images;               ///< The swapchain images.
    std::vector<vk::UniqueImageView> imageViews; ///< The swapchain images' image views with a unique handle.

    std::vector<vk::UniqueFramebuffer> framebuffers; ///< The swapchain images' framebuffers with a unique handle.

    Image depthImage;                   ///< The depth image.
    vk::UniqueImageView depthImageView; ///< The depth image's image views with a unique handle.

    uint32_t currentImageIndex; ///< The current swapchain image index.
  };

  /// Retrieves the depth format supported by a given physical device.
  /// @param physicalDevice The physical device to check.
  /// @return Returns the supported depth format.
  auto getSupportedDepthFormat( vk::PhysicalDevice physicalDevice ) -> vk::Format;
} // namespace RAYEX_NAMESPACE

#endif // SWAPCHAIN_HPP
