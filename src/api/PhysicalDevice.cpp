#include "PhysicalDevice.hpp"

namespace RX
{
  void PhysicalDevice::create(VkInstance instance)
  {
    uint32_t physicalDeviceCount = 0;
    VK_ASSERT(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr), "Failed to enumerate physical devices");

    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    VK_ASSERT(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()), "Failed to enumerate physical devices");

    for (uint32_t i = 0; i < physicalDeviceCount; ++i)
    {
      VkPhysicalDeviceProperties props;
      vkGetPhysicalDeviceProperties(physicalDevices[i], &props);

      if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
      {
        physicalDevice = physicalDevices[i]; // TODO: Do proper device scoring!
        VK_LOG("GPU: " << props.deviceName);
        return;
      }
    }

    // Any non-discrete GPU will not have enough performance to do path tracing at all.
    VK_ERROR("No discrete GPU with Vulkan support available");
  }
}