#ifndef COMMAND_POOL_HPP
#define COMMAND_POOL_HPP

#include "api/BaseComponent.hpp"

namespace RX
{
  class CommandPool : public BaseComponent
  {
  public:
    CommandPool();
    ~CommandPool();
    
    inline VkCommandPool get() { return m_commandPool; }

    void initialize(VkDevice device, uint32_t queueFamilyIndex);
    void destroy();

    void reset(VkCommandPoolResetFlags flags = 0);

  private:
    VkCommandPool m_commandPool;
    VkDevice m_device;
  };
}

#endif // COMMAND_POOL_HPP