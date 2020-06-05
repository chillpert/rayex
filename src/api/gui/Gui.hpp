#ifndef GUI_HPP
#define GUI_HPP

#include "DescriptorPool.hpp"
#include "CommandPool.hpp"
#include "CommandBuffer.hpp"
#include "Framebuffer.hpp"

#include <imgui.h>
#include <imgui_impl_vulkan.h>

namespace RX
{
  struct GuiInfo
  {
    SDL_Window* window;
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    uint32_t queueFamilyIndex; // Note: is actually used for the command pools.
    VkQueue queue;  // TODO: In total there are 3 queues involved in the initialization process. This might need to be split up.
    VkPipelineCache pipelineCache = VK_NULL_HANDLE;
    uint32_t minImageCount; // Note: is a pseudo value
    uint32_t imageCount;
    VkFormat swapchainImageFormat;
    VkExtent2D swapchainImageExtent;
    std::vector<VkImageView> swapchainImageViews;
  };

  // TODO: implement ImGui_ImplVulkan_SetMinImageCount in case a swapchain suddenly has different MinImageCount.

  class Gui
  {
  public:
    RX_API virtual ~Gui();

    inline GuiInfo& getInfo() { return m_info; }
    inline CommandBuffer& getCommandBuffer() { return m_commandBuffers; }

    void initialize(GuiInfo& info);
    void beginRender();
    RX_API virtual void render();
    void endRender();
    void beginRenderPass(int index);
    void endRenderPass(int index);

    void destroy();
    void recreate();

  private:
    void initDescriptorPool();
    void initRenderPass();
    void initCommandPool();
    void initFonts();
    void initCommandBuffers();
    void initFramebuffers();

  private:
    GuiInfo m_info;

    DescriptorPool m_descriptorPool;
    CommandPool m_commandPool;
    CommandBuffer m_commandBuffers;
    VkRenderPass m_renderPass;
    std::vector<Framebuffer> m_framebuffers;
  };
}

#endif // GUI_HPP