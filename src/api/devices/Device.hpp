#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "Instance.hpp"
#include "QueueManager.hpp"

namespace RX
{
  struct DeviceInfo
  {
    vk::PhysicalDevice physicalDevice;
    std::vector<QueueFamily> queueFamilies;
    std::vector<const char*> extensions;
  };

  class Device
  {
  public:
    inline vk::Device get() { return m_device.get(); }
    inline DeviceInfo& getInfo() { return m_info; }

    void initialize(DeviceInfo& info);
    void waitIdle();

  private:
    void checkExtensionSupport() const;

    vk::UniqueDevice m_device;
    DeviceInfo m_info;

    bool m_created = false;
  };
}

#endif // DEVICE_HPP