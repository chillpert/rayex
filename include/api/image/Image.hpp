#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "pch/stdafx.hpp"

namespace RENDERER_NAMESPACE
{
  /// A wrapper class for a Vulkan image.
  /// @ingroup API
  class Image
  {
  public:
    /// @return Returns the Vulkan image object without the unique handle.
    inline const vk::Image get( ) const { return image.get( ); }

    /// @return Returns the iamge's extent.
    inline const vk::Extent3D getExtent( ) const { return extent; }

    /// @return Returns the image's format.
    inline const vk::Format getFormat( ) const { return format; }
    
    /// @return Returns the image's layout.
    inline const vk::ImageLayout getLayout( ) const { return layout; }

    /// Creates the image and allocates memory for it.
    /// @param createInfo The Vulkan image create info.
    bool init( const vk::ImageCreateInfo& createInfo );

    /// Used to transition this image's layout.
    /// @param layout The target layout.
    /// @note This function creates its own single-time usage command buffer.
    void transitionToLayout( vk::ImageLayout layout );

    /// Used to transition this image's layout.
    /// @param layout The target layout
    /// @param commandBuffer The command buffer that will be used to set up a pipeline barrier.
    /// @warning It is required to call vk::CommandBuffer::begin() or RENDERER_NAMESPACE::CommandBuffer::begin() before calling this function.
    void transitionToLayout( vk::ImageLayout layout, vk::CommandBuffer commandBuffer );
 
    /// Retrieves a supported image format for a given physical device.
    /// @param physicalDevice The physical device to check.
    /// @param formatsToTest A vector of formats that will be checked for their support.
    /// @param features The Vulkan format features.
    /// @param tiling The Vulkan image tiling.
    static vk::Format findSupportedFormat( vk::PhysicalDevice physicalDevice, const std::vector<vk::Format>& formatsToTest, vk::FormatFeatureFlagBits features, vk::ImageTiling tiling );

  protected:
    vk::UniqueImage image; ///< The Vulkan image with a unique handle.
    vk::UniqueDeviceMemory memory; ///< The image's memory with a unique handle.

    vk::Extent3D extent; ///< The image's extent.
    vk::Format format; ///< The image's format.
    vk::ImageLayout layout; ///< THe image's layout.
  };
}

#endif // IMAGE_HPP