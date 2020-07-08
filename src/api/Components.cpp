#include "Components.hpp"

namespace rx
{
  std::shared_ptr<WindowBase> g_window = nullptr;
  vk::Instance g_instance = nullptr;
  vk::PhysicalDevice g_physicalDevice = nullptr;
  vk::Device g_device = nullptr;
  vk::SurfaceKHR g_surface = nullptr;
  std::unique_ptr<vk::DispatchLoaderDynamic> g_dispatchLoaderDynamic = nullptr;
  vk::SwapchainKHR g_swapchain = nullptr;
  uint32_t g_swapchainImageCount = 0;
  vk::Format g_surfaceFormat;

  vk::CommandPool g_graphicsCmdPool = nullptr;
  vk::CommandPool g_transferCmdPool = nullptr;
  vk::Queue g_graphicsQueue = nullptr;
  vk::Queue g_transferQueue = nullptr;
  uint32_t g_graphicsFamilyIndex;
  uint32_t g_transferFamilyIndex;
}