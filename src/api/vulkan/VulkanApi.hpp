#ifndef VULKAN_API_HPP
#define VULKAN_API_HPP

#include "api/Api.hpp"
#include "api/vulkan/Instance.hpp"
#include "api/vulkan/Surface.hpp"
#include "api/vulkan/Device.hpp"
#include "api/vulkan/SwapChain.hpp"
#include "api/vulkan/Pipeline.hpp"
#include "api/vulkan/CommandBuffer.hpp"

namespace RX
{
  class VulkanApi : public Api
  {
  public:
    VulkanApi();

  private:
    void initialize(std::shared_ptr<Window> window) override;
    void update() override;
    void render() override;
    void clean() override;

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

    Instance m_instance;
    Surface m_surface;
    Device m_device;
    SwapChain m_swapChain;
    Pipeline m_pipeline;
    CommandBuffer m_commandBuffer;
  };
}

#endif // VULKAN_API_HPP