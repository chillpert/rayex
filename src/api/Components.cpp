#include "Components.hpp"

namespace rx
{
  std::shared_ptr<WindowBase> g_window = nullptr;
  vk::Instance g_instance = nullptr;
  vk::PhysicalDevice g_physicalDevice = nullptr;
  vk::Device g_device = nullptr;
  vk::SurfaceKHR g_surface = nullptr;
  vk::SwapchainKHR g_swapchain = nullptr;
  uint32_t g_swapchainImageCount = 0;
  vk::Format g_surfaceFormat;
  std::vector<vk::UniqueImageView> g_swapchainImageViews;
  uint32_t g_shaderGroups = 0;
  vk::DynamicLoader g_dynamicLoader;

  vk::CommandPool g_graphicsCmdPool = nullptr;
  vk::CommandPool g_transferCmdPool = nullptr;
  vk::Queue g_graphicsQueue = nullptr;
  vk::Queue g_transferQueue = nullptr;
  uint32_t g_graphicsFamilyIndex;
  uint32_t g_transferFamilyIndex;

  vk::PhysicalDeviceLimits g_physicalDeviceLimits;
}