#ifndef VULKAN_API_HPP
#define VULKAN_API_HPP

#include "api/Api.hpp"
#include "api/vulkan/DeviceManager.hpp"

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
    bool checkExtensionSupport(const char** sdlExtensions, uint32_t sdlExtensionsCount);
    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions();
    void createDevices();
    void createSurface();

    VkInstance m_instance;
    VkSurfaceKHR m_surface;
    DeviceManager m_deviceManager;
  };
}

#endif // VULKAN_API_HPP