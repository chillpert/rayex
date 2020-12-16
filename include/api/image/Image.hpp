#pragma once

#include "pch/stdafx.hpp"

namespace RAYEX_NAMESPACE
{
  /// A wrapper class for a Vulkan image.
  /// @ingroup API
  class Image
  {
  public:
    auto get( ) const -> vk::Image { return _image.get( ); }

    auto getExtent( ) const -> vk::Extent3D { return _extent; }

    auto getFormat( ) const -> vk::Format { return _format; }

    auto getLayout( ) const -> vk::ImageLayout { return _layout; }

    /// Creates the image and allocates memory for it.
    /// @param createInfo The Vulkan image create info.
    void init( const vk::ImageCreateInfo& createInfo );

    /// Used to transition this image's layout.
    /// @param layout The target layout.
    /// @param subresourceRange Optionally used to define a non-standard subresource range.
    /// @note This function creates its own single-time usage command buffer.
    void transitionToLayout( vk::ImageLayout layout, const vk::ImageSubresourceRange* subresourceRange = nullptr );

    /// Used to transition this image's layout using an already existing command buffer.
    /// @param layout The target layout
    /// @param commandBuffer The command buffer that will be used to set up a pipeline barrier.
    /// @param subresourceRange Optionally used to define a non-standard subresource range.
    void transitionToLayout( vk::ImageLayout layout, vk::CommandBuffer commandBuffer, const vk::ImageSubresourceRange* subresourceRange = nullptr );

  protected:
    vk::UniqueImage _image;
    vk::UniqueDeviceMemory _memory;

    vk::Extent3D _extent;
    vk::Format _format;
    vk::ImageLayout _layout;
  };
} // namespace RAYEX_NAMESPACE
