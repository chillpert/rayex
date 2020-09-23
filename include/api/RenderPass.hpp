#ifndef RENDER_PASS_HPP
#define RENDER_PASS_HPP

#include "pch/stdafx.hpp"

namespace RENDERER_NAMESPACE
{
  /// A wrapper class for a Vulkan render pass.
  /// @ingroup API
  class RenderPass
  {
  public:
    /// @return Returns the Vulkan render pass without the unique handle.
    inline const vk::RenderPass get( ) const { return renderPass.get( ); }

    /// Initializes the Vulkan render pass.
    /// @param attachments The Vulkan attachment description.
    /// @param subpasses The Vulkan subpass description.
    /// @param dependencies The Vulkan subpass dependencies.
    /// @param initialize If true, the render pass object will be initialized right away without an additional call to init().  
    /// @return Returns true if initialization was successful.
    bool init( const std::vector<vk::AttachmentDescription>& attachments, const std::vector<vk::SubpassDescription>& subpasses, const std::vector<vk::SubpassDependency>& dependencies );

    /// Call to begin the render pass.
    /// @param framebuffer The swapchain framebuffer.
    /// @param commandBuffer The command buffer used to begin the render pass.
    /// @param renderArea Defines the size of the render area.
    /// @param clearValues The clear values.
    /// @note RENDERER_NAMESPACE::CommandBuffer::begin() or vk::CommandBuffer::begin() must have been already called prior to calling this function.
    void begin( vk::Framebuffer framebuffer, vk::CommandBuffer commandBuffer, vk::Rect2D renderArea, const std::vector<vk::ClearValue>& clearValues ) const;
    
    /// Call to end the render pass.
    /// @param commandBuffer 
    void end( vk::CommandBuffer commandBuffer ) const;

  private:
    vk::UniqueRenderPass renderPass; ///< The Vulkan render pass with a unique handle.
  };
}

#endif // RENDER_PASS_HPP