#ifndef GUI_HPP
#define GUI_HPP

#include "DescriptorPool.hpp"

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
    uint32_t queueFamilyIndex; // Note: Might be a pseudo value
    VkQueue queue;
    VkPipelineCache pipelineCache = VK_NULL_HANDLE;
    uint32_t minImageCount; // Note: is a pseudo value
    uint32_t imageCount;
    VkFormat swapchainImageFormat;
    VkCommandPool commandPool;
  };

  // TODO: implement ImGui_ImplVulkan_SetMinImageCount in case a swapchain suddenly has different MinImageCount.

  class Gui
  {
  public:
    ~Gui();

    void initialize(GuiInfo& gui);
    void begin();
    void render();
    void end();

    void destroy();
    void recreate();

  private:
    void initDescriptorPool();
    void initRenderPass();

  private:
    GuiInfo m_info;

    DescriptorPool m_descriptorPool;
    VkRenderPass m_renderPass;
  };
}

#endif // GUI_HPP