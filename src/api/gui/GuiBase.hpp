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
  /// A cass to create an ImGui-based GUI.
  /// 
  /// This class acts like an interface for the client to create their own GUI. 
  /// It is possible to create multiple GUI objects and re-assign them to the renderer at runtime.
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
  /// class CustomGui : public GuiBase
  /// {
  /// private:
  ///   // Configure style and input of the GUI.
  ///   void configure() override { }
  /// 
  ///   // Put the rendering commands in here.
  ///   void render() override { }
  /// };
  /// 
  /// Renderer myRenderer;
  /// myRenderer.init( );
  /// 
  /// // Put this line after the declaration of the rx::Renderer object.
  /// auto myGui = std::make_shared<CustomGui>( );
  /// myRenderer.setGui( myGui );
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /// @warning The GUI object must be declared after the renderer to ensure the correct order for destroying and freeing resources.
  /// @ingroup Base API
  class GuiBase
  {
  public:
    GuiBase( ) = default;

    /// @param surface A pointer to a rx::Surface object.
    /// @param swapchainImageExtent The extent of the swapchain images.
    /// @param swapchainImageViews The swapchain images' image views.
    /// @param initialize If true, the GUI will be initialized right away without an additional call to init().
    GuiBase( const Surface* const surface, vk::Extent2D swapchainImageExtent, const std::vector<vk::ImageView>& swapchainImageViews, bool initialize = true );

    RX_API virtual ~GuiBase( ) = default;
    
    /// Returns the GUI's command buffer at the given index.
    /// @param index The index of the command buffer to access.
    /// @return Returns the Vulkan command buffer.
    inline const vk::CommandBuffer getCommandBuffer( uint32_t index ) const { return m_commandBuffers.get( index ); }

    /// Used to configure all ImGui settings.
    /// 
    /// The client should override this function if they want to change the style or various other configuration settings.
    RX_API virtual void configure( );

    /// This function is for calling the individual ImGui components, e.g. widgets.
    /// 
    /// The client should override this function to create their own GUI.
    RX_API virtual void render( );

    /// Creates the GUI and all required Vulkan components.
    /// @param surface A pointer to a rx::Surface object.
    /// @param swapchainImageExtent The extent of the swapchain images.
    /// @param swapchainImageViews The swapchain images' image views.
    void init( const Surface* const surface, vk::Extent2D swapchainImageExtent, const std::vector<vk::ImageView>& swapchainImageViews );

    /// Used to recreate the GUI in case the window size was changed.
    /// @param swapchainImageExtent The extent of the swapchain images.
    /// @param swapchainImageViews The swapchain images' image views.
    void recreate( vk::Extent2D swapchainImageExtent, const std::vector<vk::ImageView>& swapchainImageViews );

    /// Creates a new ImGui frame.
    void newFrame( );

    /// Calls the actual ImGui render call.
    void endRender( );

    /// Records the ImGui rendering calls to the command buffer at the given image index.
    /// @param imageIndex The index addressing a command buffer.
    void renderDrawData( uint32_t imageIndex );

    /// Destroys all ImGui resources.
    RX_API void destroy( );

  private:
    /// Creates a descriptor pool.
    void initDescriptorPool( );

    /// Creates a render pass for the GUI.
    /// @param surface A pointer to a rx::Surface object.
    void initRenderPass( const Surface* const surface );

    /// Creates a command pool for the GUI's command buffers.
    void initCommandPool( );

    /// Creates the ImGui font textures.
    void initFonts( );

    /// Creates the command buffers to record the ImGui rendering commands to.
    void initCommandBuffers( );

    /// Creates framebuffers for the render pass.
    /// @param swapchainImageViews A vector holding the swapchain images' image view.
    void initFramebuffers( const std::vector<vk::ImageView>& swapchainImageViews );

  private:
    vk::UniqueDescriptorPool m_descriptorPool; ///< A Vulkan descriptor pool with a unique handle.
    vk::UniqueCommandPool m_commandPool; ///< A Vulkan command pool with a unique handle.
    CommandBuffer m_commandBuffers; ///< A rx::CommandBuffer.
    RenderPass m_renderPass; ///< A Vulkan render pass.
    std::vector<vk::UniqueFramebuffer> m_framebuffers; ///< A vector of Vulkan framebuffers with unique handles.

    vk::Extent2D m_swapchainImageExtent; ///< The swapchain images' image extent.
  };
}

#endif // GUI_HPP