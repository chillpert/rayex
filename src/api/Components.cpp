#include "Components.hpp"

namespace RX
{
  std::shared_ptr<WindowBase> g_window = nullptr;
  vk::Instance g_instance = nullptr;
  vk::PhysicalDevice g_physicalDevice = nullptr;
  vk::Device g_device = nullptr;
  vk::SurfaceKHR g_surface = nullptr;

  vk::CommandPool g_graphicsCmdPool = nullptr;
  vk::CommandPool g_transferCmdPool = nullptr;

  std::unique_ptr<vk::DispatchLoaderDynamic> g_dispatchLoaderDynamic = nullptr;
}