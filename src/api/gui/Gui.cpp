#include "Gui.hpp"
#include "CommandBuffer.hpp"

namespace RX
{
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
    init_info.CheckVkResultFn = check_vk_result;

    initRenderPass();
    ImGui_ImplVulkan_Init(&init_info, m_renderPass);

    // Command pool dedicated for ImGui.
    CommandPoolInfo commandPoolInfo{ };
    commandPoolInfo.device = info.device;
    commandPoolInfo.queueFamilyIndex = info.queueFamilyIndex; // This should definitely be the a graphics queue family index.
    commandPoolInfo.createFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    m_commandPool.initialize(commandPoolInfo);

    // Fonts
    CommandBufferInfo singleTimeCommandBufferInfo{ };
    singleTimeCommandBufferInfo.device = info.device;
    singleTimeCommandBufferInfo.commandPool = m_commandPool.get();
    singleTimeCommandBufferInfo.queue = info.queue;
    singleTimeCommandBufferInfo.freeAutomatically = true;
    singleTimeCommandBufferInfo.componentName = "command buffer for ImGui font creation";

    CommandBuffer commandBuffer;
    commandBuffer.initialize(singleTimeCommandBufferInfo);
    commandBuffer.begin();
      ImGui_ImplVulkan_CreateFontsTexture(commandBuffer.getFront());
    commandBuffer.end();
    
    // Create command buffers for each image in the swapchain.
    CommandBufferInfo commandBufferInfo{ };
    commandBufferInfo.device = info.device;
    commandBufferInfo.commandPool = m_commandPool.get();
    commandBufferInfo.queue = info.queue; // A graphics queue from the index of the command pool.
    commandBufferInfo.commandBufferCount = info.imageCount;
    commandBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferInfo.freeAutomatically = false;
    //commandBufferInfo.submitAutomatically = false;
    commandBufferInfo.usageFlags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    commandBufferInfo.componentName = "command buffers for ImGui";

    m_commandBuffers.initialize(commandBufferInfo);

    FramebufferInfo framebufferInfo{ };
    framebufferInfo.device = info.device;
    framebufferInfo.renderPass = m_renderPass;
    framebufferInfo.extent = info.swapchainImageExtent;

    m_framebuffers.resize(info.imageCount);
    for (size_t i = 0; i < m_framebuffers.size(); ++i)
    {
      framebufferInfo.imageView = info.swapchainImageViews[i];
      m_framebuffers[i].initialize(framebufferInfo);
    }
  }

  void Gui::render()
  {
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL2_NewFrame(m_info.window);
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    ImGui::Render();
  }

  void Gui::beginRenderPass(int index)
  {
    {
      m_commandPool.reset();   
      m_commandBuffers.begin(index);
    }
    {
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
  }


  void Gui::endRenderPass(int index)
  {
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_commandBuffers.get()[index]);

    vkCmdEndRenderPass(m_commandBuffers.get()[index]);

    m_commandBuffers.end(index);


  }

  void Gui::recreate()
  {

  }

  void Gui::destroy()
  {
    // TODO:
  }

  void Gui::initDescriptorPool()
  {
    DescriptorPoolInfo info{ };
    info.device = m_info.device;
    info.maxSets = m_info.imageCount; // TODO: might be wrong.
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
}