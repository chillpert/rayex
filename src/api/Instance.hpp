#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include "pch/stdafx.hpp"
#include "window/Window.hpp"

namespace RX
{
  class Instance
  {
  public:
    void createInstance(std::shared_ptr<Window> window);

    inline vk::UniqueInstance& getInstance() { return m_instance; }

  private:
    bool checkExtensionSupport(const char** sdlExtensions, uint32_t sdlExtensionsCount);
    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions(std::shared_ptr<Window> window);

    vk::UniqueInstance m_instance;
  };
}

#endif // INSTANCE_HPP