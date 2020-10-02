#ifndef DESTRUCTORS_HPP
#define DESTRUCTORS_HPP

#include "pch/stdafx.hpp"

namespace vk
{
  /// A collection of functions to destroy and free any given resource.
  /// @ingroup API
  namespace Destructor
  {
    /// Destroy an image view.
    /// @param imageView The image view that should be deleted.
    void destroyImageView( ImageView imageView );

    /// Destroy a vector of image views.
    /// @param imageViews The image views that should be deleted.
    void destroyImageViews( std::vector<ImageView>& imageViews );

    /// Destroy a descriptor pool.
    /// @param descriptorPool The descriptor pool that should be deleted.
    void destroyDescriptorPool( DescriptorPool descriptorPool );

    /// Destroy a command pool.
    /// @param commandPool The command pool that should be deleted.
    void destroyCommandPool( CommandPool commandPool );

    /// Destroy a framebuffer.
    /// @param framebuffer The framebuffer that should be deleted.
    void destroyFramebuffer( Framebuffer framebuffer );

    /// Destroy a vector of framebuffers.
    /// @param framebuffer The framebuffers that should be deleted.
    void destroyFramebuffers( std::vector<Framebuffer>& framebuffers );

    /// Destroy a query pool.
    /// @param queryPool The query pool that should be deleted.
    void destroyQueryPool( QueryPool queryPool );

    /// Destroy a shader module.
    /// @param shaderModule The shader shader module that should be deleted.
    void destroyShaderModule( ShaderModule shaderModule );

    /// Free allocated memory.
    /// @param memory The memory that should be freed..
    void freeMemory( DeviceMemory memory );
  } // namespace Destructor
} // namespace vk

#endif // DESTRUCTORS_HPP