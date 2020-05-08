#ifndef VULKAN_API_HPP
#define VULKAN_API_HPP

#include "api/Instance.hpp"
#include "api/Surface.hpp"
#include "api/Device.hpp"
#include "api/SwapChain.hpp"
#include "api/Pipeline.hpp"
#include "api/CommandBuffer.hpp"
#include "window/Window.hpp"

namespace RX
{
  class Api
  {
  public:
    Api();

    void initialize(Window* window);
    void update();
    void render();
    void clean();

  private:
    void createInstance();
    void createDevices();
    void createSurface();
    void createSwapChain();
    void createImageViews();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffers();
    void createSemaphores();

    void recreateSwapChain();

    Instance m_instance;
    Surface m_surface;
    Device m_device;
    SwapChain m_swapChain;
    Pipeline m_pipeline;
    CommandBuffer m_commandBuffer;

    Window* m_window;
  };
}

#endif // VULKAN_API_HPP