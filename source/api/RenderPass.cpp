#include "api/RenderPass.hpp"
#include "api/misc/Components.hpp"

namespace RENDERER_NAMESPACE
{
  RenderPass::RenderPass( const std::vector<vk::AttachmentDescription>& attachments, const std::vector<vk::SubpassDescription>& subpasses, const std::vector<vk::SubpassDependency>& dependencies, bool initialize )
  {
    if ( initialize )
      init( attachments, subpasses, dependencies );
  }

  void RenderPass::init( const std::vector<vk::AttachmentDescription>& attachments, const std::vector<vk::SubpassDescription>& subpasses, const std::vector<vk::SubpassDependency>& dependencies )
  {
    vk::RenderPassCreateInfo createInfo( { },                                             // flags
                                         static_cast< uint32_t >( attachments.size( ) ),  // attachmentCount
                                         attachments.data( ),                             // pAttachments
                                         static_cast< uint32_t >( subpasses.size( ) ),    // subpassCount
                                         subpasses.data( ),                               // pSubpasses
                                         static_cast< uint32_t >( dependencies.size( ) ), // dependencyCount
                                         dependencies.data( ) );                          // pDependencies
    
    m_renderPass = g_device.createRenderPassUnique( createInfo );
    RX_ASSERT( m_renderPass, "Failed to create render pass." );
  }

  void RenderPass::begin( vk::Framebuffer framebuffer, vk::CommandBuffer commandBuffer, vk::Rect2D renderArea, const std::vector<vk::ClearValue>& clearValues ) const
  {
    vk::RenderPassBeginInfo beginInfo( m_renderPass.get( ),                            // renderPass
                                       framebuffer,                                    // framebuffer
                                       renderArea,                                     // renderArea
                                       static_cast< uint32_t >( clearValues.size( ) ), // clearValueCount
                                       clearValues.data( ) );                          // pClearValues

    commandBuffer.beginRenderPass( beginInfo, vk::SubpassContents::eInline );
  }

  void RenderPass::end( vk::CommandBuffer commandBuffer ) const
  {
    commandBuffer.endRenderPass( ); // CMD
  }
}