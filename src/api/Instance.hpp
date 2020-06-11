#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include "window/Window.hpp"

namespace RX
{
  struct InstanceInfo
  {
    std::shared_ptr<Window> window;
    std::vector<const char*> layers;
    std::vector<const char*> extensions;
  };

  class Instance
  {
  public:
    inline vk::Instance get() { return m_instance.get(); }
    inline InstanceInfo& getInfo() { return m_info; }

    void initialize(InstanceInfo& info);

  private:
    void checkLayersSupport();
    void checkExtensionsSupport();

    uint32_t getApiVersion();

    vk::UniqueInstance m_instance;
    InstanceInfo m_info;
  };
}

#endif // INSTANCE_HPP