#ifndef GUI_HPP
#define GUI_HPP

#include "CommandBuffer.hpp"
#include "RenderPass.hpp"
#include "Swapchain.hpp"
#include "Surface.hpp"

#include <imgui.h>
#include <imgui_impl_vulkan.h>

namespace rx
{
  class GuiBase
  {
  public:
    GuiBase( ) = default;
    GuiBase( const Surface* const surface, vk::Extent2D swapchainImageExtent, const std::vector<vk::ImageView>& swapchainImageViews, bool initialize = true );
    RX_API virtual ~GuiBase( ) = default;
    
    inline const vk::CommandBuffer getCommandBuffer( uint32_t index ) const { return m_commandBuffers.get( index ); }

    RX_API virtual void configure( );
    RX_API virtual void render( );

    void init( const Surface* const surface, vk::Extent2D swapchainImageExtent, const std::vector<vk::ImageView>& swapchainImageViews );
    void recreate( vk::Extent2D swapchainImageExtent, const std::vector<vk::ImageView>& swapchainImageViews );

    void newFrame( );
    void endRender( );
    void renderDrawData( uint32_t imageIndex );

    RX_API void destroy( );

  private:
    void initDescriptorPool( );
    void initRenderPass( const Surface* const surface );
    void initCommandPool( );
    void initFonts( );
    void initCommandBuffers( );
    void initFramebuffers( const std::vector<vk::ImageView>& swapchainImageViews );

  private:
    vk::UniqueDescriptorPool m_descriptorPool;
    vk::UniqueCommandPool m_commandPool;
    CommandBuffer m_commandBuffers;
    RenderPass m_renderPass;
    std::vector<vk::UniqueFramebuffer> m_framebuffers;

    vk::Extent2D m_swapchainImageExtent;
  };
}

#endif // GUI_HPP