#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "base/Window.hpp"

namespace RAYEX_NAMESPACE
{
  namespace components
  {
    extern std::shared_ptr<Window> window;                       ///< Reference to the window.
    extern vk::Instance instance;                                ///< Reference to the Vulkan instance.
    extern vk::PhysicalDevice physicalDevice;                    ///< Reference to the Vulkan physical device.
    extern vk::Device device;                                    ///< Reference to the Vulkan logical device.
    extern vk::SurfaceKHR surface;                               ///< Reference to the Vulkan surface.
    extern vk::SwapchainKHR swapchain;                           ///< Reference to the Vulkan swapchain.
    extern uint32_t swapchainImageCount;                         ///< Variable to store the Vulkan swapchain's image count.
    extern vk::Format surfaceFormat;                             ///< Reference to the Vulkan surface's format.
    extern std::vector<vk::UniqueImageView> swapchainImageViews; ///< Reference to the Vulkan swapchain image views.
    extern uint32_t shaderGroups;                                ///< Variable to store the Vulkan shader groups.
    extern vk::DynamicLoader dynamicLoader;                      ///< Reference to the dynamic loader used to load function pointers provided by extensions.

    extern vk::CommandPool graphicsCmdPool; ///< Reference to a Vulkan graphics command pool.
    extern vk::CommandPool transferCmdPool; ///< Reference to a Vulkan transfer command pool.
    extern vk::Queue graphicsQueue;         ///< Reference to a Vulkan graphics queue.
    extern vk::Queue transferQueue;         ///< Reference to a Vulkan transfer queue.
    extern uint32_t graphicsFamilyIndex;    ///< stores a Vulkan graphics family index.
    extern uint32_t transferFamilyIndex;    ///< stores a Vulkan transfer family index.

    extern vk::PhysicalDeviceLimits physicalDeviceLimits; ///< Global variable to store the physical device's limits.
    extern uint32_t frameCount;                           ///< Stores the current frame count required by the jitter cam.

    const size_t maxGeometryInstances = 2000; ///< Stores the maximum amount of geometry nodes.
    const size_t maxTextures          = 2000; ///< Stores the maximum amount of textures.
    const size_t maxDirectionalLights = 2000; ///< Stores the maximum amount of directional light sources.
    const size_t maxPointLights       = 2000; ///< Stores the maximum amount of point light sources supported.
    const size_t maxGeometries        = 512;  ///< Stores the maximum amount of models supported.

    extern std::string assetsPath; ///< Path to where shaders, models and textures are stored.

  } // namespace components
} // namespace RAYEX_NAMESPACE

#endif // COMPONENTS_HPP
