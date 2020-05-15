#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "BaseComponent.hpp"
#include "Instance.hpp"
#include "QueueManager.hpp"

namespace RX
{
  class Device : public BaseComponent
  {
  public:
    Device();

    inline VkDevice get() { return m_device; }

    void initialize(VkPhysicalDevice physicalDevice, QueueManager& queueManager);
    void destroy();

    // To verify the support of any device extension, use PhysicalDevice::checkExtensionSupport.
    void pushExtension(const char* name);

  private:
    VkDevice m_device;

    std::vector<const char*> m_extensions;
  };
}

#endif // DEVICE_HPP