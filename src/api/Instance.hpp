#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include "BaseComponent.hpp"
#include "window/Window.hpp"

namespace RX
{
  class Instance : public BaseComponent
  {
  public:
    Instance();
    ~Instance();

    inline VkInstance get() { return m_instance; }

    inline std::vector<const char*>& getLayers() { return m_layers; }
    inline std::vector<const char*>& getExtension() { return m_extensions; }

    // Needs to be called before create
    void pushLayer(const char* name);
    // Needs to be called before create
    void pushExtension(const char* name);

    void initialize(const std::shared_ptr<Window> window);
    void destroy();

    void print();

  private:
    void checkLayerSupport(const char* name);
    void checkExtensionSupport(const char* name);

    uint32_t getApiVersion();

    VkInstance m_instance;

    std::vector<const char*> m_layers;
    std::vector<const char*> m_extensions;
  };
}

#endif // INSTANCE_HPP