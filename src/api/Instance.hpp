#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include "window/Window.hpp"

namespace RX
{
  class Instance
  {
  public:
    inline VkInstance get() { return instance; }

    inline std::vector<const char*>& getLayers() { return layers; }
    inline std::vector<const char*>& getExtension() { return extensions; }

    // Needs to be called before create
    void pushLayer(const char* name);
    // Needs to be called before create
    void pushExtension(const char* name);

    void create(const std::shared_ptr<Window> window);
    void destroy();

    void print();

  private:
    void checkLayerSupport(const char* name);
    void checkExtensionSupport(const char* name);

    uint32_t getApiVersion();

    VkInstance instance;

    std::vector<const char*> layers;
    std::vector<const char*> extensions;

    // Structs
    VkApplicationInfo appInfo;
    VkInstanceCreateInfo createInfo;
  };
}

#endif // INSTANCE_HPP