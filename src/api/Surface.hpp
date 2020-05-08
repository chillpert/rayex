#ifndef SURFACE_HPP
#define SURFACE_HPP

#include "pch/stdafx.hpp"
#include "window/Window.hpp"

namespace RX
{
  class Surface
  {
  public:
    void createSurface(Window* window, VkInstance* instance);
    void destroySurface();

    inline VkSurfaceKHR* getSurface() { return &m_surface; }

  private:
    VkSurfaceKHR m_surface;

    // pointer to VulkanApi class member
    VkInstance* m_instance;
  };
}

#endif // SURFACE_HPP