#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "base/Window.hpp"

namespace RAYEXEC_NAMESPACE
{
  extern std::shared_ptr<Window> g_window;                       ///< Reference to the window.
  extern vk::Instance g_instance;                                ///< Reference to the Vulkan instance.
  extern vk::PhysicalDevice g_physicalDevice;                    ///< Reference to the Vulkan physical device.
  extern vk::Device g_device;                                    ///< Reference to the Vulkan logical device.
  extern vk::SurfaceKHR g_surface;                               ///< Reference to the Vulkan surface.
  extern vk::SwapchainKHR g_swapchain;                           ///< Reference to the Vulkan swapchain.
  extern uint32_t g_swapchainImageCount;                         ///< Variable to store the Vulkan swapchain's image count.
  extern vk::Format g_surfaceFormat;                             ///< Reference to the Vulkan surface's format.
  extern std::vector<vk::UniqueImageView> g_swapchainImageViews; ///< Reference to the Vulkan swapchain image views.
  extern uint32_t g_shaderGroups;                                ///< Variable to store the Vulkan shader groups.
  extern vk::DynamicLoader g_dynamicLoader;                      ///< Reference to the dynamic loader used to load function pointers provided by extensions.

  extern vk::CommandPool g_graphicsCmdPool; ///< Reference to a Vulkan graphics command pool.
  extern vk::CommandPool g_transferCmdPool; ///< Reference to a Vulkan transfer command pool.
  extern vk::Queue g_graphicsQueue;         ///< Reference to a Vulkan graphics queue.
  extern vk::Queue g_transferQueue;         ///< Reference to a Vulkan transfer queue.
  extern uint32_t g_graphicsFamilyIndex;    ///< stores a Vulkan graphics family index.
  extern uint32_t g_transferFamilyIndex;    ///< stores a Vulkan transfer family index.

  extern vk::PhysicalDeviceLimits g_physicalDeviceLimits; ///< Global variable to store the physical device's limits.

  extern uint32_t g_modelCount;
  RX_API extern uint32_t g_frameCount;

  const size_t g_maxGeometryInstances     = 2000; ///< Stores the maximum amount of geometry nodes.
  const size_t g_maxTextures              = 2000; ///< Stores the maximum amount of textures.
  const size_t g_maxDirectionalLightNodes = 2000; ///< Stores the maximum amount of directional light sources.
  const size_t g_maxPointLightNodes       = 2000; ///< Stores the maximum amount of point light sources supported.
  const size_t g_maxModels                = 2000; ///< Stores the maximum amount of models supported.

  extern std::string g_assetsPath; ///< Path to where shaders, models and textures are stored.
} // namespace RAYEXEC_NAMESPACE

#endif // COMPONENTS_HPP
