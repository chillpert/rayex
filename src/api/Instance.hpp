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
    ~Instance();

    inline VkInstance get() { return m_instance; }
    inline InstanceInfo& getInfo() { return m_info; }

    void initialize(InstanceInfo& info);

  private:
    void destroy();

    void checkLayersSupport();
    void checkExtensionsSupport();

    uint32_t getApiVersion();

    VkInstance m_instance;
    InstanceInfo m_info;

    bool m_created = false;
  };
}

#endif // INSTANCE_HPP