#include "Gui.hpp"
#include "api/buffers/CommandBuffer.hpp"

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
    clean();
  }

  void Gui::initialize(
    VkInstance instance, 
    VkPhysicalDevice physicalDevice, 
    VkDevice device,
    Queues& queues, 
    VkDescriptorPool descriptorPool,
    Surface& surface,
    VkRenderPass renderPass,
    SDL_Window* window,
    uint32_t imageCount
  )
  {
    m_window = window;

    auto surfaceCapabilities = surface.getInfo().capabilities;
    uint32_t minImageCount = surfaceCapabilities.minImageCount + 1;
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForVulkan(window);
    ImGui_ImplVulkan_InitInfo init_info = { };
    init_info.Instance = instance;
    init_info.PhysicalDevice = physicalDevice;
    init_info.Device = device;
    init_info.QueueFamily = queues.getGraphicsFamilyIndex();
    init_info.Queue = queues.getGraphicsQueue();
    init_info.PipelineCache = nullptr;
    init_info.DescriptorPool = descriptorPool;
    init_info.Allocator = nullptr;
    init_info.MinImageCount = minImageCount;
    init_info.ImageCount = imageCount;
    init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info, renderPass);

    CommandPool commandPool;
    commandPool.initialize(device, queues.getGraphicsFamilyIndex());

    commandPool.reset();

    CommandBuffer commandBuffer;
    commandBuffer.begin(device, commandPool.get(), queues.getGraphicsQueue());

    ImGui_ImplVulkan_CreateFontsTexture(commandBuffer.get());

    commandBuffer.end();
  
    vkDeviceWaitIdle(device);

    ImGui_ImplVulkan_DestroyFontUploadObjects();

    m_commandPool.initialize(device, queues.getGraphicsFamilyIndex());
  }

  void Gui::beginRender()
  {
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL2_NewFrame(m_window);
    ImGui::NewFrame();
  }

  void Gui::render()
  {
    ImGui::Render();
    m_drawData = ImGui::GetDrawData();
  }

  void Gui::endRender(VkDevice device, Queues& queues, VkRenderPass renderPass, VkFramebuffer framebuffer)
  {
    m_commandPool.reset();

    CommandBuffer commandBuffer;
    commandBuffer.begin(device, m_commandPool.get(), queues.getGraphicsQueue());

    VkRenderPassBeginInfo info{ };
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.renderPass = renderPass;
    info.framebuffer = framebuffer;
    info.renderArea.extent.width = 900;
    info.renderArea.extent.height = 600;
    info.clearValueCount = 1;

    VkClearValue colorValue;
    colorValue.color =	{0.3f, 0.3f, 0.3f, 1.0f};
    VkClearValue depthValue;
    depthValue.depthStencil = {1.0f, 0};

    VkClearValue clearValues[] = { colorValue, depthValue };
    info.clearValueCount = 2;
    info.pClearValues = clearValues;

    vkCmdBeginRenderPass(commandBuffer.get(), &info, VK_SUBPASS_CONTENTS_INLINE);

    ImGui_ImplVulkan_RenderDrawData(m_drawData, commandBuffer.get());

    vkCmdEndRenderPass(commandBuffer.get());

    commandBuffer.end();
  }

  void Gui::renderDemo()
  {
    if (ImGui::Begin("Vulkan Example"))
    {
      ImGui::Text("Hello there!");
    }
    ImGui::End();
  }

  void Gui::recreate()
  {

  }

  void Gui::clean()
  {
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
  }
}