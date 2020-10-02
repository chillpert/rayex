#include "base/Gui.hpp"

#include "api/buffers/CommandBuffer.hpp"
#include "api/utility/Destructors.hpp"
#include "api/utility/Initializers.hpp"

namespace RAYEXEC_NAMESPACE
{
  void Gui::configure( )
  {
    IMGUI_CHECKVERSION( );
    ImGui::CreateContext( );
    ImGuiIO& io = ImGui::GetIO( );
    (void) io;
  }

  void Gui::init( const Surface* const surface, vk::Extent2D swapchainImageExtent, const std::vector<vk::ImageView>& swapchainImageViews )
  {
    this->swapchainImageExtent = swapchainImageExtent;

    configure( );

    RX_ASSERT( ImGui_ImplSDL2_InitForVulkan( g_window->get( ) ), "Failed to init ImGui for Vulkan" );

    initDescriptorPool( );

    ImGui_ImplVulkan_InitInfo init_info { };
    init_info.Instance       = g_instance;
    init_info.PhysicalDevice = g_physicalDevice;
    init_info.Device         = g_device;
    init_info.QueueFamily    = g_graphicsFamilyIndex;
    init_info.Queue          = g_graphicsQueue;
    init_info.PipelineCache  = nullptr;
    init_info.DescriptorPool = this->descriptorPool.get( );
    init_info.Allocator      = nullptr;
    init_info.MinImageCount  = surface->getCapabilities( ).minImageCount + 1;
    init_info.ImageCount     = static_cast<uint32_t>( g_swapchainImageCount );

    initRenderPass( surface );
    RX_ASSERT( ImGui_ImplVulkan_Init( &init_info, this->renderPass.get( ) ), "Failed to initialize Vulkan for ImGui." );
    initCommandPool( );
    initFonts( );
    initCommandBuffers( );
    initFramebuffers( swapchainImageViews );
  }

  void Gui::recreate( vk::Extent2D swapchainImageExtent, const std::vector<vk::ImageView>& swapchainImageViews )
  {
    this->swapchainImageExtent = swapchainImageExtent;

    initFramebuffers( swapchainImageViews );
  }

  void Gui::newFrame( )
  {
    ImGui_ImplVulkan_NewFrame( );
    ImGui_ImplSDL2_NewFrame( g_window->get( ) );
    ImGui::NewFrame( );
  }

  void Gui::render( )
  {
    ImGui::ShowDemoWindow( );
  }

  void Gui::endRender( )
  {
    ImGui::Render( );
  }

  void Gui::renderDrawData( uint32_t imageIndex )
  {
    this->commandBuffers.begin( imageIndex );
    this->renderPass.begin( this->framebuffers[imageIndex].get( ), this->commandBuffers.get( imageIndex ), { 0, this->swapchainImageExtent }, { { std::array<float, 4> { 0.5f, 0.5, 0.5f, 1.0f } } } );

    ImGui_ImplVulkan_RenderDrawData( ImGui::GetDrawData( ), this->commandBuffers.get( )[imageIndex] );

    this->renderPass.end( this->commandBuffers.get( imageIndex ) );
    this->commandBuffers.end( imageIndex );
  }

  void Gui::destroy( )
  {
    this->commandBuffers.free( );

    ImGui_ImplVulkan_Shutdown( );
    ImGui_ImplSDL2_Shutdown( );
    ImGui::DestroyContext( );
  }

  void Gui::initCommandPool( )
  {
    this->commandPool = vk::Initializer::initCommandPoolUnique( g_graphicsFamilyIndex, vk::CommandPoolCreateFlagBits::eResetCommandBuffer );
  }

  void Gui::initDescriptorPool( )
  {
    std::vector<vk::DescriptorPoolSize> poolSizes = {
      { vk::DescriptorType::eSampler, 1000 },
      { vk::DescriptorType::eCombinedImageSampler, 1000 },
      { vk::DescriptorType::eSampledImage, 1000 },
      { vk::DescriptorType::eStorageImage, 1000 },
      { vk::DescriptorType::eUniformTexelBuffer, 1000 },
      { vk::DescriptorType::eStorageTexelBuffer, 1000 },
      { vk::DescriptorType::eUniformBuffer, 1000 },
      { vk::DescriptorType::eStorageBuffer, 1000 },
      { vk::DescriptorType::eUniformBufferDynamic, 1000 },
      { vk::DescriptorType::eStorageBufferDynamic, 1000 },
      { vk::DescriptorType::eInputAttachment, 1000 }
    };

    this->descriptorPool = vk::Initializer::initDescriptorPoolUnique( poolSizes, g_swapchainImageCount );
  }

  void Gui::initRenderPass( const Surface* const surface )
  {
    vk::AttachmentDescription attachment( { },                               // flags
                                          surface->getFormat( ),             // format
                                          vk::SampleCountFlagBits::e1,       // samples
                                          vk::AttachmentLoadOp::eLoad,       // loadOp
                                          vk::AttachmentStoreOp::eStore,     // storeOp
                                          vk::AttachmentLoadOp::eDontCare,   // stencilLoadOp
                                          vk::AttachmentStoreOp::eDontCare,  // stencilStoreOp
                                          vk::ImageLayout::ePresentSrcKHR,   // initialLayout
                                          vk::ImageLayout::ePresentSrcKHR ); // finalLayout

    vk::AttachmentReference colorAttachment( 0,                                          // attachment
                                             vk::ImageLayout::eColorAttachmentOptimal ); // layout

    vk::SubpassDescription subpass( { },                              // flags
                                    vk::PipelineBindPoint::eGraphics, // pipelineBindPoint
                                    0,                                // inputAttachmentsCount
                                    nullptr,                          // pInputAttachments
                                    1,                                // colorAttachmentsCount
                                    &colorAttachment,                 // pColorAttachments
                                    nullptr,                          // pResolveAttachments
                                    nullptr,                          // pDepthStencilAttachment
                                    0,                                // preserveAttachemntCount
                                    nullptr );                        // pPreserveAttachments

    vk::SubpassDependency dependency( VK_SUBPASS_EXTERNAL,                               // srcSubpass
                                      0,                                                 // dstSubpass
                                      vk::PipelineStageFlagBits::eColorAttachmentOutput, // srcStageMask
                                      vk::PipelineStageFlagBits::eColorAttachmentOutput, // dstStageMask
                                      vk::AccessFlagBits::eColorAttachmentWrite,         // srcAccessMask
                                      vk::AccessFlagBits::eColorAttachmentWrite,         // dstAccessMask
                                      { } );                                             // dependencyFlags

    vk::RenderPassCreateInfo info( { },           // flags
                                   1,             // attachmentCount
                                   &attachment,   // pAttachments
                                   1,             // subpassCount
                                   &subpass,      // pSubpasses
                                   1,             // dependencyCount
                                   &dependency ); // pDependencies

    this->renderPass.init( { attachment }, { subpass }, { dependency } );
  }

  void Gui::initFonts( )
  {
    CommandBuffer commandBuffer( this->commandPool.get( ) );
    commandBuffer.begin( );

    RX_ASSERT( ImGui_ImplVulkan_CreateFontsTexture( commandBuffer.get( 0 ) ), "Failed to create ImGui fonts texture." );

    commandBuffer.end( );
    commandBuffer.submitToQueue( g_graphicsQueue );
  }

  void Gui::initCommandBuffers( )
  {
    // Create command buffers for each image in the swapchain.
    this->commandBuffers.init( this->commandPool.get( ), g_swapchainImageCount, vk::CommandBufferUsageFlagBits::eRenderPassContinue );
  }

  void Gui::initFramebuffers( const std::vector<vk::ImageView>& swapchainImageViews )
  {
    this->framebuffers.resize( static_cast<uint32_t>( swapchainImageViews.size( ) ) );
    for ( size_t i = 0; i < this->framebuffers.size( ); ++i )
      this->framebuffers[i] = vk::Initializer::initFramebufferUnique( { swapchainImageViews[i] }, this->renderPass.get( ), this->swapchainImageExtent );
  }
} // namespace RAYEXEC_NAMESPACE