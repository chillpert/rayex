#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include "window/Window.hpp"

namespace RX
{
  class Instance
  {
  public:
    inline VkInstance get() { return m_instance; }

    // Needs to be called before create
    void pushLayer(const char* name);
    // Needs to be called before create
    void pushExtension(const char* name);

    void create(const std::shared_ptr<Window> const window);

  private:
    void checkLayerSupport(const char* name);
    void checkExtensionSupport(const char* name);

    VkInstance m_instance;

    std::vector<const char*> m_layers;
    std::vector<const char*> m_extensions;
  };
}

#endif // INSTANCE_HPP