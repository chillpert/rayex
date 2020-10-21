#include "api/Components.hpp"

namespace RAYEX_NAMESPACE
{
  namespace components
  {
    vk::Instance instance             = nullptr;
    vk::PhysicalDevice physicalDevice = nullptr;
    vk::Device device                 = nullptr;
    vk::SurfaceKHR surface            = nullptr;
    vk::SwapchainKHR swapchain        = nullptr;
    uint32_t swapchainImageCount      = 0;
    vk::DynamicLoader dynamicLoader;

    vk::CommandPool graphicsCmdPool = nullptr;
    vk::CommandPool transferCmdPool = nullptr;
    vk::Queue graphicsQueue         = nullptr;
    vk::Queue transferQueue         = nullptr;
    uint32_t graphicsFamilyIndex;
    uint32_t transferFamilyIndex;

    vk::PhysicalDeviceLimits physicalDeviceLimits;

    uint32_t frameCount = 0;

    std::string assetsPath;

  } // namespace components
} // namespace RAYEX_NAMESPACE
