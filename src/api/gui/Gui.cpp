#include "Gui.hpp"
#include "CommandBuffer.hpp"

namespace RX
{
  CommandBufferInfo commandBufferInfo{ };
  ImGui_ImplVulkan_InitInfo init_info{ };

  static void check_vk_result(VkResult err)
  {
    if (err == 0)
      return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
      abort();
  }

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
    init_info.CheckVkResultFn = check_vk_result;

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

    VkRenderPassBeginInfo info{ };
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.renderPass = m_renderPass;
    info.framebuffer = m_framebuffers[index].get();
    info.renderArea.extent = m_info.swapchainImageExtent;
    info.clearValueCount = 1;

    VkClearValue clearValue;
    clearValue.color = { 0.5f, 0.5, 0.5f, 1.0f };
    info.pClearValues = &clearValue;

    vkCmdBeginRenderPass(m_commandBuffers.get()[index], &info, VK_SUBPASS_CONTENTS_INLINE);
  }

  void Gui::endRenderPass(int index)
  {
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_commandBuffers.get()[index]);

    vkCmdEndRenderPass(m_commandBuffers.get()[index]);

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
    commandPoolInfo.createFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    m_commandPool.initialize(commandPoolInfo);
  }

  void Gui::initDescriptorPool()
  {
    DescriptorPoolInfo info{ };
    info.device = m_info.device;
    info.maxSets = m_info.imageCount;
    info.poolSizes =
    {
      { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
      { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
      { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
      { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
      { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
      { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
      { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
      { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
      { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
      { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
      { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };

    m_descriptorPool.initialize(info);
  }

  void Gui::initRenderPass()
  {
    VkAttachmentDescription attachment{ };
    attachment.format = m_info.swapchainImageFormat;
    attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment{ };
    color_attachment.attachment = 0;
    color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{ };
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment;

    VkSubpassDependency dependency{ };
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;  // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo info{ };
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.attachmentCount = 1;
    info.pAttachments = &attachment;
    info.subpassCount = 1;
    info.pSubpasses = &subpass;
    info.dependencyCount = 1;
    info.pDependencies = &dependency;

    VK_ASSERT(vkCreateRenderPass(m_info.device, &info, nullptr, &m_renderPass), "Failed to create render pass for GUI.");
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
    commandBufferInfo.device = m_info.device;
    commandBufferInfo.commandPool = m_commandPool.get();
    commandBufferInfo.queue = m_info.queue; // A graphics queue from the index of the command pool.
    commandBufferInfo.commandBufferCount = m_info.imageCount;
    commandBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferInfo.freeAutomatically = false;
    commandBufferInfo.usageFlags = 0;
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