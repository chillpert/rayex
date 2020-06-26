#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include "WindowBase.hpp"

namespace RX
{
  struct InstanceInfo
  {
    std::shared_ptr<WindowBase> window;
    std::vector<const char*> layers;
    std::vector<const char*> extensions;
  };

  class Instance
  {
  public:
    Instance() = default;
    Instance(InstanceInfo& info);
    Instance(InstanceInfo&& info);

    inline vk::Instance get() { return m_instance.get(); }

    void init(InstanceInfo& info);
    void init(InstanceInfo&& info);

  private:
    void checkLayersSupport();
    void checkExtensionsSupport();

    uint32_t getApiVersion();

    vk::UniqueInstance m_instance;
    InstanceInfo m_info;
  };
}

#endif // INSTANCE_HPP