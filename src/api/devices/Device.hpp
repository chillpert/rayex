#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "Instance.hpp"
#include "QueueManager.hpp"

namespace RX
{
  class Device
  {
  public:
    Device() = default;
    Device(const std::vector<QueueFamily>& queueFamilies, std::vector<const char*>& extensions, bool initialize = true);

    /*
      Creates the logical device and enables all device extensions provided if they are supported.
      @param queueFamilies - All queue families whose queues should be created.
      @param extensions - All device extensions that should be enabled.
    */
    void init(const std::vector<QueueFamily>& queueFamilies, std::vector<const char*>& extensions);

  private:
    void checkExtensionSupport(const std::vector<const char*>& extensions) const;

    vk::UniqueDevice m_device;
  };
}

#endif // DEVICE_HPP