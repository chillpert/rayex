#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "WindowBase.hpp"

namespace rx
{
  extern std::shared_ptr<WindowBase> g_window; ///< Global reference to the window.
  extern vk::Instance g_instance; ///< Global reference to the Vulkan instance.
  extern vk::PhysicalDevice g_physicalDevice; ///< Global reference to the Vulkan physical device.
  extern vk::Device g_device; ///< Global reference to the Vulkan logical device.
  extern vk::SurfaceKHR g_surface; ///< Global reference to the Vulkan surface.
  extern vk::SwapchainKHR g_swapchain; ///< Global reference to the Vulkan swapchain.
  extern uint32_t g_swapchainImageCount; ///< Global variable to store the Vulkan swapchain's image count.
  extern vk::Format g_surfaceFormat; ///< Global reference to the Vulkan surface's format.
  extern std::vector<vk::UniqueImageView> g_swapchainImageViews; ///< Global reference to the Vulkan swapchain image views.
  extern uint32_t g_shaderGroups; ///< Global variable to store the Vulkan shader groups.
  extern vk::DynamicLoader g_dynamicLoader; ///< Global reference to the dynamic loader used to load function pointers provided by extensions.

  extern vk::CommandPool g_graphicsCmdPool; ///< Global reference to a Vulkan graphics command pool.
  extern vk::CommandPool g_transferCmdPool; ///< Global reference to a Vulkan transfer command pool.
  extern vk::Queue g_graphicsQueue; ///< Global reference to a Vulkan graphics queue.
  extern vk::Queue g_transferQueue; ///< Global reference to a Vulkan transfer queue.
  extern uint32_t g_graphicsFamilyIndex; ///< Global variable to store a Vulkan graphics family index.
  extern uint32_t g_transferFamilyIndex; ///< Global variable to store a Vulkan transfer family index.

  extern vk::PhysicalDeviceLimits g_physicalDeviceLimits; ///< Global variable to store the physical device's limits.

  /// @note Vulkan default allocation limit equals 4096 allocations.
  const size_t g_maxGeometryNodes = 2000; ///< Global variable to store the maximum amount of geometry nodes supported by the renderer.
  const size_t g_maxTextures = 2000; ///< Global variable to store the maximum amount of textures supported by the renderer.
  const size_t g_maxLightNodes = 25; ///< Global variable to store the maximum amount of light sources supported by the renderer.
}

#endif // COMPONENTS_HPP