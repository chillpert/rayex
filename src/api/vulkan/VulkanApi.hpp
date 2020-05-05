#ifndef VULKAN_API_HPP
#define VULKAN_API_HPP

#include "api/Api.hpp"

namespace RX
{
  class VulkanApi : public Api
  {
  private:
    void initialize(std::shared_ptr<Window> window) override;
    void update() override;
    void render() override;
    void clean() override;

    void assertVulkan(VkResult result, const char* message);

    void createInstance();


    VkInstance m_instance;
  };
}

#endif // VULKAN_API_HPP