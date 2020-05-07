#ifndef VULKAN_API_HPP
#define VULKAN_API_HPP

#include "api/Api.hpp"
#include "api/vulkan/Instance.hpp"
#include "api/vulkan/Surface.hpp"
#include "api/vulkan/DeviceManager.hpp"
#include "api/vulkan/SwapChain.hpp"
#include "api/vulkan/GraphicsPipeline.hpp"

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

    Instance m_instance;
    Surface m_surface;
    DeviceManager m_deviceManager;
    SwapChain m_swapChain;
    GraphicsPipeline m_graphicsPipeline;
  };
}

#endif // VULKAN_API_HPP