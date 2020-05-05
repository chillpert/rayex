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

    void assertVulkan(VkResult result, const char* message);

    void createInstance();
    bool checkExtensionSupport(const char** sdlExtensions, uint32_t sdlExtensionsCount);
    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions();

    VkInstance m_instance;
    DeviceManager m_deviceManager;

    const std::vector<const char*> m_validationLayers;
  };
}

#endif // VULKAN_API_HPP