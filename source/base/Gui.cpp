#include "base/Gui.hpp"
#include "api/buffers/CommandBuffer.hpp"
#include "api/utility/Initializers.hpp"
#include "api/utility/Destructors.hpp"

namespace RENDERER_NAMESPACE
{
  void Gui::configure( )
  {
    IMGUI_CHECKVERSION( );
    ImGui::CreateContext( );
    ImGuiIO& io = ImGui::GetIO( ); (void) io;
  }

  bool Gui::init( const Surface* const surface, vk::Extent2D swapchainImageExtent, const std::vector<vk::ImageView>& swapchainImageViews )
  {
    this->swapchainImageExtent = swapchainImageExtent;

    configure( );

    bool result = ImGui_ImplSDL2_InitForVulkan( g_window->get( ) );
    RX_ASSERT_INIT( result );

    result = initDescriptorPool( );
    RX_ASSERT_INIT( result );

    ImGui_ImplVulkan_InitInfo init_info { };
    init_info.Instance = g_instance;
    init_info.PhysicalDevice = g_physicalDevice;
    init_info.Device = g_device;
    init_info.QueueFamily = g_graphicsFamilyIndex;
    init_info.Queue = g_graphicsQueue;
    init_info.PipelineCache = nullptr;
    init_info.DescriptorPool = this->descriptorPool.get( );
    init_info.Allocator = nullptr;
    init_info.MinImageCount = surface->getCapabilities( ).minImageCount + 1;
    init_info.ImageCount = static_cast<uint32_t>( g_swapchainImageCount );

    result = initRenderPass( surface );
    RX_ASSERT_INIT( result );

    result = ImGui_ImplVulkan_Init( &init_info, this->renderPass.get( ) );
    RX_ASSERT_INIT( result );

    result = initCommandPool( );
    RX_ASSERT_INIT( result );

    result = initFonts( );
    RX_ASSERT_INIT( result );

    result = initCommandBuffers( );
    RX_ASSERT_INIT( result );

    return initFramebuffers( swapchainImageViews );
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
    this->renderPass.begin( this->framebuffers[imageIndex].get( ), this->commandBuffers.get( imageIndex ), { 0, this->swapchainImageExtent }, { { std::array < float,4> { 0.5f, 0.5, 0.5f, 1.0f } } } );

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

  bool Gui::initCommandPool( )
  {
    this->commandPool = vk::Initializer::initCommandPoolUnique( g_graphicsFamilyIndex, vk::CommandPoolCreateFlagBits::eResetCommandBuffer );
    return true;
  }

  bool Gui::initDescriptorPool( )
  {
    std::vector<vk::DescriptorPoolSize> poolSizes =
    {
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
    return true;
  }

  bool Gui::initRenderPass( const Surface* const surface )
  {
    vk::AttachmentDescription attachment( { },                                      // flags
                                          surface->getFormat( ),                    // format
                                          vk::SampleCountFlagBits::e1,              // samples
                                          vk::AttachmentLoadOp::eLoad,              // loadOp
                                          vk::AttachmentStoreOp::eStore,            // storeOp
                                          vk::AttachmentLoadOp::eDontCare,          // stencilLoadOp
                                          vk::AttachmentStoreOp::eDontCare,         // stencilStoreOp
                                          vk::ImageLayout::ePresentSrcKHR,          // initialLayout
                                          vk::ImageLayout::ePresentSrcKHR );        // finalLayout          

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

    vk::SubpassDependency dependency( VK_SUBPASS_EXTERNAL,                                // srcSubpass
                                      0,                                                  // dstSubpass
                                      vk::PipelineStageFlagBits::eColorAttachmentOutput,  // srcStageMask
                                      vk::PipelineStageFlagBits::eColorAttachmentOutput,  // dstStageMask
                                      vk::AccessFlagBits::eColorAttachmentWrite,          // srcAccessMask
                                      vk::AccessFlagBits::eColorAttachmentWrite,          // dstAccessMask
                                      { } );                                              // dependencyFlags

    vk::RenderPassCreateInfo info( { },          // flags
                                   1,            // attachmentCount
                                   &attachment,  // pAttachments
                                   1,            // subpassCount
                                   &subpass,     // pSubpasses
                                   1,            // dependencyCount
                                   &dependency); // pDependencies

    return this->renderPass.init( { attachment }, { subpass }, { dependency } );
  }

  bool Gui::initFonts( )
  {
    CommandBuffer commandBuffer( this->commandPool.get( ) );
    commandBuffer.begin( );
    
    bool result = ImGui_ImplVulkan_CreateFontsTexture( commandBuffer.get( 0 ) );

    commandBuffer.end( );
    commandBuffer.submitToQueue( g_graphicsQueue );
  
    return result;
  }

  bool Gui::initCommandBuffers( )
  {
    // Create command buffers for each image in the swapchain.
    return this->commandBuffers.init( this->commandPool.get( ), 
                                      g_swapchainImageCount, 
                                      vk::CommandBufferUsageFlagBits::eRenderPassContinue );
  }

  bool Gui::initFramebuffers( const std::vector<vk::ImageView>& swapchainImageViews )
  {
    this->framebuffers.resize( static_cast<uint32_t>( swapchainImageViews.size( ) ) );
    for ( size_t i = 0; i < this->framebuffers.size( ); ++i )
    {
      this->framebuffers[i] = vk::Initializer::initFramebufferUnique( { swapchainImageViews[i] }, this->renderPass.get( ), this->swapchainImageExtent );
    }

    return true;
  }
}