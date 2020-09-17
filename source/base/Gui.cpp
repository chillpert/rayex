#include "base/Gui.hpp"
#include "api/buffers/CommandBuffer.hpp"
#include "api/utility/Initializers.hpp"
#include "api/utility/Destructors.hpp"

namespace RENDERER_NAMESPACE
{
  Gui::Gui( const Surface* const surface, vk::Extent2D swapchainImageExtent, const std::vector<vk::ImageView>& swapchainImageViews, bool initialize )
  {
    if ( initialize )
      init( surface, swapchainImageExtent, swapchainImageViews );
  }

  void Gui::configure( )
  {
    IMGUI_CHECKVERSION( );
    ImGui::CreateContext( );
    ImGuiIO& io = ImGui::GetIO( ); (void) io;
  }

  void Gui::init( const Surface* const surface, vk::Extent2D swapchainImageExtent, const std::vector<vk::ImageView>& swapchainImageViews )
  {
    m_swapchainImageExtent = swapchainImageExtent;

    configure( );

    ImGui_ImplSDL2_InitForVulkan( g_window->get( ) );
    initDescriptorPool( );

    ImGui_ImplVulkan_InitInfo init_info { };
    init_info.Instance = g_instance;
    init_info.PhysicalDevice = g_physicalDevice;
    init_info.Device = g_device;
    init_info.QueueFamily = g_graphicsFamilyIndex;
    init_info.Queue = g_graphicsQueue;
    init_info.PipelineCache = nullptr;
    init_info.DescriptorPool = m_descriptorPool.get( );
    init_info.Allocator = nullptr;
    init_info.MinImageCount = surface->getCapabilities( ).minImageCount + 1;
    init_info.ImageCount = static_cast<uint32_t>( g_swapchainImageCount );

    initRenderPass( surface );
    ImGui_ImplVulkan_Init( &init_info, m_renderPass.get( ) );
    initCommandPool( );
    initFonts( );
    initCommandBuffers( );
    initFramebuffers( swapchainImageViews );
  }

  void Gui::recreate( vk::Extent2D swapchainImageExtent, const std::vector<vk::ImageView>& swapchainImageViews )
  {
    m_swapchainImageExtent = swapchainImageExtent;

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
    m_commandBuffers.begin( imageIndex );
    m_renderPass.begin( m_framebuffers[imageIndex].get( ), m_commandBuffers.get( imageIndex ), { 0, m_swapchainImageExtent }, { { std::array < float,4> { 0.5f, 0.5, 0.5f, 1.0f } } } );

    ImGui_ImplVulkan_RenderDrawData( ImGui::GetDrawData( ), m_commandBuffers.get( )[imageIndex] );

    m_renderPass.end( m_commandBuffers.get( imageIndex ) );
    m_commandBuffers.end( imageIndex );
  }

  void Gui::destroy( )
  {
    m_commandBuffers.free( );

    ImGui_ImplVulkan_Shutdown( );
    ImGui_ImplSDL2_Shutdown( );
    ImGui::DestroyContext( );
  }

  void Gui::initCommandPool( )
  {
    m_commandPool = vk::Initializer::createCommandPoolUnique( g_graphicsFamilyIndex, vk::CommandPoolCreateFlagBits::eResetCommandBuffer );
  }

  void Gui::initDescriptorPool( )
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

    m_descriptorPool = vk::Initializer::createDescriptorPoolUnique( poolSizes, g_swapchainImageCount );
  }

  void Gui::initRenderPass( const Surface* const surface )
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

    m_renderPass.init( { attachment }, { subpass }, { dependency } );
  }

  void Gui::initFonts( )
  {
    CommandBuffer commandBuffer( m_commandPool.get( ) );
    commandBuffer.begin( );
    ImGui_ImplVulkan_CreateFontsTexture( commandBuffer.get( 0 ) );
    commandBuffer.end( );
    commandBuffer.submitToQueue( g_graphicsQueue );
  }

  void Gui::initCommandBuffers( )
  {
    // Create command buffers for each image in the swapchain.
    m_commandBuffers.init( m_commandPool.get( ), 
                           g_swapchainImageCount, 
                           vk::CommandBufferUsageFlagBits::eRenderPassContinue );
  }

  void Gui::initFramebuffers( const std::vector<vk::ImageView>& swapchainImageViews )
  {
    m_framebuffers.resize( static_cast<uint32_t>( swapchainImageViews.size( ) ) );
    for ( size_t i = 0; i < m_framebuffers.size( ); ++i )
    {
      m_framebuffers[i] = vk::Initializer::createFramebufferUnique( { swapchainImageViews[i] }, m_renderPass.get( ), m_swapchainImageExtent );
    }
  }
}