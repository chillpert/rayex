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
    ~Gui();

    inline VkCommandBuffer& getCommandBuffer(size_t index) { return m_commandBuffers.get()[index]; }

    void initialize(GuiInfo& gui);
    void render();
    void beginRenderPass(int index);
    void endRenderPass(int index);

    void destroy();
    void recreate();

  private:
    void initDescriptorPool();
    void initRenderPass();

  private:
    GuiInfo m_info;

    DescriptorPool m_descriptorPool;
    VkRenderPass m_renderPass;

    CommandPool m_commandPool;
    CommandBuffer m_commandBuffers;
    std::vector<Framebuffer> m_framebuffers;
  };
}

#endif // GUI_HPP