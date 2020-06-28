#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "WindowBase.hpp"

namespace RX
{
  // Unique
  extern std::shared_ptr<WindowBase> g_window;
  extern vk::Instance g_instance;
  extern vk::PhysicalDevice g_physicalDevice;
  extern vk::Device g_device;
  extern vk::SurfaceKHR g_surface;
  extern std::unique_ptr<vk::DispatchLoaderDynamic> g_dispatchLoaderDynamic;

  // Not unique
  extern vk::CommandPool g_graphicsCmdPool;
  extern vk::CommandPool g_transferCmdPool;
}

#endif // COMPONENTS_HPP