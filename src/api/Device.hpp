#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct QueueFamilyIndices
  {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
  };

  class Device
  {
  public:
    Device(vk::UniqueInstance* instance);

    void pickPhysicalDevice();
    void createLogicalDevice();

    void destroyLogicalDevice();

    inline VkPhysicalDevice* getPhysicalDevice() { return &m_physicalDevice; }
    inline VkDevice* getLogicalDevice() { return &m_logicalDevice; }

    static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    
    inline VkQueue* getGraphicsQueue() { return &m_graphicsQueue; }
    inline VkQueue* getPresentQueue() { return &m_presentQueue; }
  
  private:
    // Returns a score for any given physical device, based on supported features.
    // It will also check for queue family support, device extension support and swap chain support.
    size_t evaluatePhysicalDevice(VkPhysicalDevice device);

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    VkPhysicalDeviceProperties getPhysicalDeviceProperties(VkPhysicalDevice device);
    VkPhysicalDeviceFeatures getPhysicalDeviceFeatures(VkPhysicalDevice device);

    void printPhysicalDeviceInfo();
    
    VkPhysicalDevice m_physicalDevice;
    VkDevice m_logicalDevice;

    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;

    // pointer to VulkanApi class members
    vk::UniqueInstance* m_instance;
  };
}

#endif // DEVICE_HPP