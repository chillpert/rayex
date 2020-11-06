#pragma once

#include "pch/stdafx.hpp"

namespace RAYEX_NAMESPACE
{
  /// A wrapper class for a Vulkan image.
  /// @ingroup API
  class Image
  {
  public:
    /// @return Returns the Vulkan image object without the unique handle.
    auto get( ) const -> vk::Image { return _image.get( ); }

    /// @return Returns the iamge's extent.
    auto getExtent( ) const -> vk::Extent3D { return _extent; }

    /// @return Returns the image's format.
    auto getFormat( ) const -> vk::Format { return _format; }

    /// @return Returns the image's layout.
    auto getLayout( ) const -> vk::ImageLayout { return _layout; }

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
    /// @warning It is required to call vk::CommandBuffer::begin() or RAYEX_NAMESPACE::CommandBuffer::begin() before calling this function.
    void transitionToLayout( vk::ImageLayout layout, vk::CommandBuffer commandBuffer );

  protected:
    vk::UniqueImage _image;         ///< The Vulkan image with a unique handle.
    vk::UniqueDeviceMemory _memory; ///< The image's memory with a unique handle.

    vk::Extent3D _extent;    ///< The image's extent.
    vk::Format _format;      ///< The image's format.
    vk::ImageLayout _layout; ///< THe image's layout.
  };
} // namespace RAYEX_NAMESPACE
