#include "GuiBase.hpp"
#include "CommandBuffer.hpp"

namespace RX
{
  GuiBase::GuiBase(GuiInfo& info)
  {
    init(info);
  }

  GuiBase::GuiBase(GuiInfo&& info)
  {
    init(info);
  }

  GuiBase::~GuiBase()
  {
    if (m_created)
      destroy();
  }

  void GuiBase::configure()
  {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
  }

  void GuiBase::init(GuiInfo& info)
  {
    m_info = info;

    configure();

    ImGui_ImplSDL2_InitForVulkan(info.window);
    
    ImGui_ImplVulkan_InitInfo init_info{ };
    init_info.Instance = info.instance;
    init_info.PhysicalDevice = info.physicalDevice;
    init_info.Device = info.device;
    init_info.QueueFamily = info.queueFamilyIndex;
    init_info.Queue = info.queue;
    init_info.PipelineCache = info.pipelineCache;
    initDescriptorPool(); 
    init_info.DescriptorPool = m_descriptorPool.get();
    init_info.Allocator = nullptr;
    init_info.MinImageCount = info.minImageCount;
    init_info.ImageCount = info.imageCount;

    initRenderPass();
    ImGui_ImplVulkan_Init(&init_info, m_renderPass.get());

    initCommandPool();
    initFonts();
    initCommandBuffers();
    initFramebuffers();

    // Initialize render pass begin info.
    vk::ClearValue clearValue;
    clearValue.color = std::array<float, 4>{ 0.5f, 0.5, 0.5f, 1.0f };

    RenderPassBeginInfo beginInfo{ };
    beginInfo.renderArea.extent = m_info.swapchainImageExtent;
    beginInfo.clearValues = { clearValue };
    beginInfo.commandBuffers = m_commandBuffers.get();

    for (Framebuffer& framebuffer : m_framebuffers)
      beginInfo.framebuffers.push_back(framebuffer.get());

    m_renderPass.setBeginInfo(beginInfo);

    m_created = true;
  }

  void GuiBase::init(GuiInfo&& info)
  {
    init(info);
  }

  void GuiBase::beginRender()
  {
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL2_NewFrame(m_info.window);
    ImGui::NewFrame();
  }

  void GuiBase::render()
  {
    ImGui::ShowDemoWindow();
  }

  void GuiBase::endRender()
  {
    ImGui::Render();
  }

  void GuiBase::beginRenderPass(int index)
  {
    m_commandBuffers.begin(index);
    m_renderPass.begin(index);
  }

  void GuiBase::endRenderPass(int index)
  {
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_commandBuffers.get()[index]);

    m_renderPass.end(index);
    m_commandBuffers.end(index);
  }

  void GuiBase::recreate(GuiRecreateInfo& info)
  {
    m_info.minImageCount = info.minImageCount;
    m_info.imageCount = info.imageCount;
    m_info.swapchainImageFormat = info.swapchainImageFormat;
    m_info.swapchainImageExtent = info.swapchainImageExtent;
    m_info.swapchainImageViews = info.swapchainImageViews;

    destroy();

    // Re-initialize
    init(m_info);
  }

  void GuiBase::destroy()
  {
    // Clean up
    m_commandBuffers.free();
    m_commandPool.destroy(); // TODO unncessary

    for (Framebuffer& framebuffer : m_framebuffers)
      framebuffer.destroy();

    m_descriptorPool.destroy();

    m_renderPass.destroy();

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    m_created = false;
  }

  void GuiBase::initCommandPool()
  {
    m_commandPool.init(m_info.queueFamilyIndex, vk::CommandPoolCreateFlagBits::eResetCommandBuffer, { });
  }

  void GuiBase::initDescriptorPool()
  {
    m_descriptorPool.init({
      .device = m_info.device,
      .poolSizes = {
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
        },
      .maxSets = m_info.imageCount
      }
    );
  }

  void GuiBase::initRenderPass()
  {
    vk::AttachmentDescription attachment;
    attachment.format = m_info.swapchainImageFormat;
    attachment.samples = vk::SampleCountFlagBits::e1;
    attachment.loadOp = vk::AttachmentLoadOp::eLoad;
    attachment.storeOp = vk::AttachmentStoreOp::eStore;
    attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    attachment.initialLayout = vk::ImageLayout::eColorAttachmentOptimal;
    attachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

    vk::AttachmentReference color_attachment;
    color_attachment.attachment = 0;
    color_attachment.layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::SubpassDescription subpass;
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment;

    vk::SubpassDependency dependency;
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite; // or 0
    dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;

    vk::RenderPassCreateInfo info;
    info.attachmentCount = 1;
    info.pAttachments = &attachment;
    info.subpassCount = 1;
    info.pSubpasses = &subpass;
    info.dependencyCount = 1;
    info.pDependencies = &dependency;

    RenderPassInfo renderPassInfo{ };
    renderPassInfo.physicalDevice = m_info.physicalDevice;
    renderPassInfo.device = m_info.device;
    renderPassInfo.attachments = { attachment };
    renderPassInfo.subpasses = { subpass };
    renderPassInfo.dependencies = { dependency };

    m_renderPass.init(renderPassInfo);
  }

  void GuiBase::initFonts()
  {
    CmdBuffer commandBuffer({ m_info.device, m_commandPool.get() });
    commandBuffer.begin();
      ImGui_ImplVulkan_CreateFontsTexture(commandBuffer.getFront());
    commandBuffer.end();
    commandBuffer.submitToQueue(m_info.queue);
  }

  void GuiBase::initCommandBuffers()
  {
    // Create command buffers for each image in the swapchain.
    CmdBufferInfo commandBufferInfo{ };
    commandBufferInfo.device = m_info.device;
    commandBufferInfo.commandPool = m_commandPool.get();
    commandBufferInfo.commandBufferCount = m_info.imageCount;
    commandBufferInfo.level = vk::CommandBufferLevel::ePrimary;
    commandBufferInfo.usageFlags = vk::CommandBufferUsageFlagBits::eRenderPassContinue;

    m_commandBuffers.init(commandBufferInfo);
  }

  void GuiBase::initFramebuffers()
  {
    FramebufferInfo framebufferInfo{
      .device = m_info.device,
      .renderPass = m_renderPass.get(),
      .extent = m_info.swapchainImageExtent
    };

    m_framebuffers.resize(m_info.imageCount);
    for (size_t i = 0; i < m_framebuffers.size(); ++i)
    {
      framebufferInfo.imageView = m_info.swapchainImageViews[i];
      m_framebuffers[i].init(framebufferInfo);
    }
  }
}