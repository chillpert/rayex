#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "api/buffers/CommandBuffers.hpp"
#include "api/Instance.hpp"
#include "api/Queues.hpp"

namespace RX
{
  class Device
  {
  public:
    ~Device();

    inline VkDevice get() { return m_device; }

    void initialize(VkPhysicalDevice physicalDevice, Queues& queues);

    // To verify the support of any device extension, use PhysicalDevice::checkExtensionSupport.
    void pushExtension(const char* name);
    void waitIdle();

  private:
    void destroy();

    VkDevice m_device;

    std::vector<const char*> m_extensions;
  };
}

#endif // DEVICE_HPP