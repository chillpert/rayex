#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "Instance.hpp"
#include "QueueManager.hpp"

namespace RX
{
  struct DeviceInfo
  {
    VkPhysicalDevice physicalDevice;
    std::vector<QueueFamily> queueFamilies;
    std::vector<const char*> extensions;
  };

  class Device
  {
  public:
    ~Device();

    inline VkDevice get() { return m_device; }
    inline DeviceInfo& getInfo() { return m_info; }

    void initialize(DeviceInfo& info);
    void waitIdle();

  private:
    void destroy();
    void checkExtensionSupport() const;

    VkDevice m_device;
    DeviceInfo m_info;

    bool m_created = false;
  };
}

#endif // DEVICE_HPP