#include "api/VulkanApi.hpp"

namespace RX
{
  void VulkanApi::initialize(std::shared_ptr<Window> window)
  {
    Api::initialize(window);

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::cout << extensionCount << " extensions supported\n";
  }

  void VulkanApi::update()
  {
    
  }

  void VulkanApi::render()
  {

  }

  void VulkanApi::clean()
  {

  }
}