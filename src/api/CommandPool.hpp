#ifndef COMMAND_POOL_HPP
#define COMMAND_POOL_HPP

#include "BaseComponent.hpp"

namespace RX
{
  class CommandPool : public BaseComponent
  {
  public:
    CommandPool();
    
    inline VkCommandPool get() { return m_commandPool; }

    void initialize(VkDevice device, uint32_t queueFamilyIndex);
    void destroy(VkDevice device);

    void reset(VkDevice device, VkCommandPoolResetFlags flags = 0);

  private:
    VkCommandPool m_commandPool;
  };
}

#endif // COMMAND_POOL_HPP