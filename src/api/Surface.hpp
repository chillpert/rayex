#ifndef SURFACE_HPP
#define SURFACE_HPP

#include "pch/stdafx.hpp"
#include "window/Window.hpp"

namespace RX
{
  class Surface
  {
  public:
    void createSurface(std::shared_ptr<Window> window, vk::UniqueInstance* instance);
    void destroySurface();

    inline VkSurfaceKHR* getSurface() { return &m_surface; }

  private:
    VkSurfaceKHR m_surface;

    // pointer to VulkanApi class member
    vk::UniqueInstance* m_instance;
  };
}

#endif // SURFACE_HPP