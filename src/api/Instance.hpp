#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include "pch/stdafx.hpp"
#include "window/Window.hpp"

namespace RX
{
  class Instance
  {
  public:
    void createInstance(Window* window);
    void destroyInstance();

    inline VkInstance* getInstance() { return &m_instance; }

  private:
    bool checkExtensionSupport(const char** sdlExtensions, uint32_t sdlExtensionsCount);
    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions(Window* window);

    VkInstance m_instance;
  };
}

#endif // INSTANCE_HPP