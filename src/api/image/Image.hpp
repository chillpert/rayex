#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "stdafx.hpp"

namespace rx
{
  /// A wrapper class for a Vulkan image.
  /// @ingroup API
  class Image
  {
  public:
    Image( ) = default;

    /// @param createInfo The Vulkan image create info.
    /// @param If true, the image will be initialized right away without an additional call to init().
    Image( const vk::ImageCreateInfo& createInfo, bool initialize = true );

    /// @return Returns the Vulkan image object without the unique handle.
    inline const vk::Image get( ) const { return m_image.get( ); }

    /// @return Returns the iamge's extent.
    inline const vk::Extent3D getExtent( ) const { return m_extent; }

    /// @return Returns the image's format.
    inline const vk::Format getFormat( ) const { return m_format; }
    
    /// @return Returns the image's layout.
    inline const vk::ImageLayout getLayout( ) const { return m_layout; }

    /// Creates the image and allocates memory for it.
    /// @param createInfo The Vulkan image create info.
    void init( const vk::ImageCreateInfo& createInfo );

    /// Used to transition this image's layout.
    /// @param layout The target layout.
    /// @note This function creates its own single-time usage command buffer.
    void transitionToLayout( vk::ImageLayout layout );

    /// Used to transition this image's layout.
    /// @param layout The target layout
    /// @param commandBuffer The command buffer that will be used to set up a pipeline barrier.
    /// @warning It is required to call vk::CommandBuffer::begin() or rx::CommandBuffer::begin() before calling this function.
    void transitionToLayout( vk::ImageLayout layout, vk::CommandBuffer commandBuffer );
 
    /// Retrieves a supported image format for a given physical device.
    /// @param physicalDevice The physical device to check.
    /// @param formatsToTest A vector of formats that will be checked for their support.
    /// @param features The Vulkan format features.
    /// @param tiling The Vulkan image tiling.
    static vk::Format findSupportedFormat( vk::PhysicalDevice physicalDevice, const std::vector<vk::Format>& formatsToTest, vk::FormatFeatureFlagBits features, vk::ImageTiling tiling );

  protected:
    vk::UniqueImage m_image; ///< The Vulkan image with a unique handle.
    vk::UniqueDeviceMemory m_memory; ///< The image's memory with a unique handle.

    vk::Extent3D m_extent; ///< The image's extent.
    vk::Format m_format; ///< The image's format.
    vk::ImageLayout m_layout; ///< THe image's layout.
  };
}

#endif // IMAGE_HPP