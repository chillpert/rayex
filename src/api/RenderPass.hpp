#ifndef RENDER_PASS_HPP
#define RENDER_PASS_HPP

#include "pch/stdafx.hpp"

namespace rx
{
  class RenderPass
  {
  public:
    RenderPass( ) = default;
    RenderPass( const std::vector<vk::AttachmentDescription>& attachments, const std::vector<vk::SubpassDescription>& subpasses, const std::vector<vk::SubpassDependency>& dependencies, bool initialize = true );
    RX_API ~RenderPass( );

    inline const vk::RenderPass get( ) const { return m_renderPass.get( ); }

    void init( const std::vector<vk::AttachmentDescription>& attachments, const std::vector<vk::SubpassDescription>& subpasses, const std::vector<vk::SubpassDependency>& dependencies );
    void destroy( );

    void begin( vk::Framebuffer framebuffer, vk::CommandBuffer CommandBuffer, vk::Rect2D renderArea, const std::vector<vk::ClearValue>& clearValues ) const;
    void end( vk::CommandBuffer CommandBuffer ) const;

  private:
    vk::UniqueRenderPass m_renderPass;
  };
}

#endif // RENDER_PASS_HPP
