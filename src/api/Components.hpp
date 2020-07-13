#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "WindowBase.hpp"

namespace rx
{
  extern std::shared_ptr<WindowBase> g_window;
  extern vk::Instance g_instance;
  extern vk::PhysicalDevice g_physicalDevice;
  extern vk::Device g_device;
  extern vk::SurfaceKHR g_surface;
  //extern std::unique_ptr<vk::DispatchLoaderDynamic> g_dispatchLoaderDynamic;
  extern vk::SwapchainKHR g_swapchain;
  extern uint32_t g_swapchainImageCount;
  extern vk::Format g_surfaceFormat;
  extern std::vector<vk::UniqueImageView> g_swapchainImageViews;
  extern uint32_t g_shaderGroups;
  extern vk::DynamicLoader g_dynamicLoader;

  extern vk::CommandPool g_graphicsCmdPool;
  extern vk::CommandPool g_transferCmdPool;
  extern vk::Queue g_graphicsQueue;
  extern vk::Queue g_transferQueue;
  extern uint32_t g_graphicsFamilyIndex;
  extern uint32_t g_transferFamilyIndex;
}

#endif // COMPONENTS_HPP