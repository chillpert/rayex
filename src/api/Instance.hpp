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
    Instance(VkInstance* instance);
    ~Instance();

    void initialize(InstanceInfo& info);
    void destroy();

    void print();

  private:
    void checkLayersSupport(const std::vector<const char*>& layers);
    void checkExtensionsSupport(const std::vector<const char*>& extensions);

    uint32_t getApiVersion();

    VkInstance* m_instance;
  };
}

#endif // INSTANCE_HPP