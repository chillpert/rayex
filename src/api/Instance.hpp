#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include "window/Window.hpp"

namespace RX
{
  class Instance
  {
  public:
    void createInstance(std::shared_ptr<Window> window);

    inline VkInstance& getInstance() { return m_instance; }

  private:
    VkInstance m_instance;
  };
}

#endif // INSTANCE_HPP