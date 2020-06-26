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

  struct GuiRecreateInfo
  {
    uint32_t minImageCount;
    uint32_t imageCount;
    vk::Format swapchainImageFormat;
    vk::Extent2D swapchainImageExtent;
    std::vector<vk::ImageView> swapchainImageViews;
  };

  class GuiBase
  {
  public:
    GuiBase() = default;
    GuiBase(GuiInfo& info);
    GuiBase(GuiInfo&& info);
    RX_API virtual ~GuiBase();

    inline CmdBuffer& getCommandBuffer() { return m_commandBuffers; }

    RX_API virtual void configure();
    RX_API virtual void render();

    void init(GuiInfo& info);
    void init(GuiInfo&& info);

    void beginRender();
    void endRender();
    void beginRenderPass(int index);
    void endRenderPass(int index);

    RX_API void destroy();
    void recreate(GuiRecreateInfo& info);

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
    CmdBuffer m_commandBuffers;
    RenderPass m_renderPass;
    std::vector<Framebuffer> m_framebuffers;

    bool m_created = false;
  };
}

#endif // GUI_HPP