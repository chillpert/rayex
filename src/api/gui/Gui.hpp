#ifndef GUI_HPP
#define GUI_HPP

#include "DescriptorPool.hpp"
#include "CommandPool.hpp"
#include "CommandBuffer.hpp"
#include "Framebuffer.hpp"
#include "RenderPass.hpp"

#include <imgui.h>
#include <imgui_impl_vulkan.h>

namespace RX
{
  struct GuiInfo
  {
    SDL_Window* window;
    vk::Instance instance;
    vk::PhysicalDevice physicalDevice;
    vk::Device device;
    uint32_t queueFamilyIndex; // Note: is actually used for the command pools.
    vk::Queue queue;  // TODO: In total there are 3 queues involved in the initialization process. This might need to be split up.
    vk::PipelineCache pipelineCache = nullptr;
    uint32_t minImageCount; // Note: is a pseudo value
    uint32_t imageCount;
    vk::Format swapchainImageFormat;
    vk::Extent2D swapchainImageExtent;
    std::vector<vk::ImageView> swapchainImageViews;
  };

  class Gui
  {
  public:
    RX_API virtual ~Gui();

    inline GuiInfo& getInfo() { return m_info; }
    inline CommandBuffer& getCommandBuffer() { return m_commandBuffers; }

    RX_API virtual void configure();
    RX_API virtual void render();

    void initialize(GuiInfo& info);
    void beginRender();
    void endRender();
    void beginRenderPass(int index);
    void endRenderPass(int index);

    RX_API void destroy();
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
    RenderPass m_renderPass;
    std::vector<Framebuffer> m_framebuffers;
  };
}

#endif // GUI_HPP