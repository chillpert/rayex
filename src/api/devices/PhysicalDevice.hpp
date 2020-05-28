#ifndef PHYSICAL_DEVICE_HPP
#define PHYSICAL_DEVICE_HPP

#include "api/BaseComponent.hpp"

namespace RX
{
  class PhysicalDevice
  {
  public:
    PhysicalDevice(VkPhysicalDevice* physicalDevice);

    // Technically not created, but enumerated. However, this way is better for the naming scheme.
    void initialize(VkInstance instance, VkSurfaceKHR surface);
  
    // Checks if all given physical device extensions are available.
    void checkExtensionSupport(const std::vector<const char*>& extensions) const;

  private:
    unsigned int evaluate(VkPhysicalDevice device, VkSurfaceKHR surface);

    VkPhysicalDevice* m_physicalDevice;
  };
}

#endif // PHYSICAL_DEVICE_HPP