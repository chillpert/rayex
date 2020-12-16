#include "base/Gui.hpp"

#include "api/Components.hpp"
#include "api/buffers/CommandBuffer.hpp"
#include "api/utility/Initializers.hpp"

namespace RAYEX_NAMESPACE
{
  void Gui::configure( )
  {
    IMGUI_CHECKVERSION( );
    ImGui::CreateContext( );
  }

  void Gui::init( SDL_Window* window, const Surface* const surface, vk::Extent2D swapchainImageExtent, vk::RenderPass renderPass )
  {
    _window               = window;
    _swapchainImageExtent = swapchainImageExtent;

    configure( );

    RX_ASSERT( ImGui_ImplSDL2_InitForVulkan( _window ), "Failed to init ImGui for Vulkan" );

    initDescriptorPool( );

    ImGui_ImplVulkan_InitInfo init_info { };
    init_info.Instance       = components::instance;
    init_info.PhysicalDevice = components::physicalDevice;
    init_info.Device         = components::device;
    init_info.QueueFamily    = components::graphicsFamilyIndex;
    init_info.Queue          = components::graphicsQueue;
    init_info.PipelineCache  = nullptr;
    init_info.DescriptorPool = _descriptorPool.get( );
    init_info.Allocator      = nullptr;
    init_info.MinImageCount  = surface->getCapabilities( ).minImageCount + 1;
    init_info.ImageCount     = static_cast<uint32_t>( components::swapchainImageCount );

    RX_ASSERT( ImGui_ImplVulkan_Init( &init_info, renderPass ), "Failed to initialize Vulkan for ImGui." );

    initFonts( );
  }

  void Gui::recreate( vk::Extent2D swapchainImageExtent )
  {
    _swapchainImageExtent = swapchainImageExtent;
  }

  void Gui::render( )
  {
    ImGui::ShowDemoWindow( );
  }

  void Gui::renderDrawData( vk::CommandBuffer commandBuffer )
  {
    ImGui_ImplSDL2_NewFrame( _window );
    ImGui::NewFrame( );

    render( );
    ImGui::Render( );

    ImGui_ImplVulkan_RenderDrawData( ImGui::GetDrawData( ), commandBuffer );
  }

  void Gui::destroy( )
  {
    ImGui_ImplVulkan_Shutdown( );
    ImGui_ImplSDL2_Shutdown( );
    ImGui::DestroyContext( );
  }

  void Gui::initDescriptorPool( )
  {
    std::vector<vk::DescriptorPoolSize> poolSizes = { { vk::DescriptorType::eSampler, 1000 },
                                                      { vk::DescriptorType::eCombinedImageSampler, 1000 },
                                                      { vk::DescriptorType::eSampledImage, 1000 },
                                                      { vk::DescriptorType::eStorageImage, 1000 },
                                                      { vk::DescriptorType::eUniformTexelBuffer, 1000 },
                                                      { vk::DescriptorType::eStorageTexelBuffer, 1000 },
                                                      { vk::DescriptorType::eUniformBuffer, 1000 },
                                                      { vk::DescriptorType::eStorageBuffer, 1000 },
                                                      { vk::DescriptorType::eUniformBufferDynamic, 1000 },
                                                      { vk::DescriptorType::eStorageBufferDynamic, 1000 },
                                                      { vk::DescriptorType::eInputAttachment, 1000 } };

    _descriptorPool = vk::Initializer::initDescriptorPoolUnique( poolSizes, components::swapchainImageCount );
  }

  void Gui::initFonts( )
  {
    _commandPool = vk::Initializer::initCommandPoolUnique( components::graphicsFamilyIndex, vk::CommandPoolCreateFlagBits::eResetCommandBuffer );

    CommandBuffer commandBuffer( _commandPool.get( ) );
    commandBuffer.begin( );

    RX_ASSERT( ImGui_ImplVulkan_CreateFontsTexture( commandBuffer.get( 0 ) ), "Failed to create ImGui fonts texture." );

    commandBuffer.end( );
    commandBuffer.submitToQueue( components::graphicsQueue );
  }
} // namespace RAYEX_NAMESPACE
