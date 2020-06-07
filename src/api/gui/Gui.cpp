#include "Gui.hpp"
#include "CommandBuffer.hpp"

namespace RX
{
  Gui::~Gui()
  {
    destroy();
  }

  void Gui::initialize(GuiInfo& info)
  {
    m_info = info;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

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
    ImGui_ImplVulkan_Init(&init_info, m_renderPass);

    initCommandPool();
    initFonts();
    initCommandBuffers();
    initFramebuffers();
  }

  void Gui::beginRender()
  {
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL2_NewFrame(m_info.window);
    ImGui::NewFrame();
  }

  void Gui::render()
  {
    ImGui::ShowDemoWindow();
  }

  void Gui::endRender()
  {
    ImGui::Render();
  }

  void Gui::beginRenderPass(int index)
  {
    m_commandBuffers.begin(index);

    vk::RenderPassBeginInfo info;
    info.renderPass = m_renderPass;
    info.framebuffer = m_framebuffers[index].get();
    info.renderArea.extent = m_info.swapchainImageExtent;
    info.clearValueCount = 1;

    vk::ClearValue clearValue;
    clearValue.color = std::array<float, 4>{ 0.5f, 0.5, 0.5f, 1.0f };
    info.pClearValues = &clearValue;

    m_commandBuffers.get()[index].beginRenderPass(info, vk::SubpassContents::eInline);
  }

  void Gui::endRenderPass(int index)
  {
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_commandBuffers.get()[index]);

    m_commandBuffers.get()[index].endRenderPass();
    m_commandBuffers.end(index);
  }

  void Gui::recreate()
  {
    // Clean up
    m_commandBuffers.free();
    m_commandPool.destroy(); // TODO unncessary
    
    for (Framebuffer& framebuffer : m_framebuffers)
      framebuffer.destroy();

    m_descriptorPool.destroy();

    destroy();

    // Re-initialize
    initialize(m_info);
  }

  void Gui::destroy()
  {
    vkDestroyRenderPass(m_info.device, m_renderPass, nullptr);
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
  }

  void Gui::initCommandPool()
  {
    CommandPoolInfo commandPoolInfo{ };
    commandPoolInfo.device = m_info.device;
    commandPoolInfo.queueFamilyIndex = m_info.queueFamilyIndex; // This should definitely be the a graphics queue family index.
    commandPoolInfo.createFlags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

    m_commandPool.initialize(commandPoolInfo);
  }

  void Gui::initDescriptorPool()
  {
    DescriptorPoolInfo info{ };
    info.device = m_info.device;
    info.maxSets = m_info.imageCount;
    info.poolSizes =
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

    m_descriptorPool.initialize(info);
  }

  void Gui::initRenderPass()
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

    m_renderPass = m_info.device.createRenderPass(info);

    if (!m_renderPass)
      RX_ERROR("Failed to create GUI render pass.");
  }

  void Gui::initFonts()
  {
    CommandBufferInfo singleTimeCommandBufferInfo{ };
    singleTimeCommandBufferInfo.device = m_info.device;
    singleTimeCommandBufferInfo.commandPool = m_commandPool.get();
    singleTimeCommandBufferInfo.queue = m_info.queue;
    singleTimeCommandBufferInfo.freeAutomatically = true;
    singleTimeCommandBufferInfo.componentName = "command buffer for ImGui font creation";

    CommandBuffer commandBuffer;
    commandBuffer.initialize(singleTimeCommandBufferInfo);
    commandBuffer.begin();
    ImGui_ImplVulkan_CreateFontsTexture(commandBuffer.getFront());
    commandBuffer.end();
  }

  void Gui::initCommandBuffers()
  {
    // Create command buffers for each image in the swapchain.
    CommandBufferInfo commandBufferInfo{ };
    commandBufferInfo.device = m_info.device;
    commandBufferInfo.commandPool = m_commandPool.get();
    commandBufferInfo.queue = m_info.queue; // A graphics queue from the index of the command pool.
    commandBufferInfo.commandBufferCount = m_info.imageCount;
    commandBufferInfo.level = vk::CommandBufferLevel::ePrimary;
    commandBufferInfo.freeAutomatically = false;
    commandBufferInfo.usageFlags = vk::CommandBufferUsageFlagBits::eRenderPassContinue;
    commandBufferInfo.componentName = "command buffers for ImGui";

    m_commandBuffers.initialize(commandBufferInfo);
  }

  void Gui::initFramebuffers()
  {
    FramebufferInfo framebufferInfo{ };
    framebufferInfo.device = m_info.device;
    framebufferInfo.renderPass = m_renderPass;
    framebufferInfo.extent = m_info.swapchainImageExtent;

    m_framebuffers.resize(m_info.imageCount);
    for (size_t i = 0; i < m_framebuffers.size(); ++i)
    {
      framebufferInfo.imageView = m_info.swapchainImageViews[i];
      m_framebuffers[i].initialize(framebufferInfo);
    }
  }
}