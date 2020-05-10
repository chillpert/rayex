#include "utils/VkUtils.hpp"

namespace RX
{
  VkInstance createInstance(std::shared_ptr<Window> window)
  {
    VkApplicationInfo appInfo = { };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo createInfo = { };
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

#ifdef RX_DEBUG
    std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

    // Checks if given validation layers are available on this device.
    uint32_t propertyCount;
    vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);

    std::vector<VkLayerProperties> properties(propertyCount);
    vkEnumerateInstanceLayerProperties(&propertyCount, properties.data());

    for (const auto& layerName : validationLayers)
    {
      bool found = false;

      for (const auto& property : properties)
      {
        std::cout << layerName << " : " << property.layerName << std::endl;

        if (strcmp(property.layerName, layerName))
        {
          found = true;
          break;
        }
      }

      if (!found)
      {
        Error::runtime("Validation layer is not available on this device", Error::API);
      }
    }

    createInfo.ppEnabledLayerNames = validationLayers.data();
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
#endif

    // Retrieves all extensions needed by SDL2 and add them to all extensions that will be used.
    uint32_t sdlExtensionsCount;
    window->getInstanceExtensions(sdlExtensionsCount, NULL);
    const char** sdlExtensionsNames = new const char*[sdlExtensionsCount];
    window->getInstanceExtensions(sdlExtensionsCount, sdlExtensionsNames);

    std::vector<const char*> extensions(sdlExtensionsNames, sdlExtensionsNames + sdlExtensionsCount);

#ifdef RX_DEBUG
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());

    VkInstance instance;

    Assert::vulkan(
      vkCreateInstance(&createInfo, nullptr, &instance),
      "Failed to create instance"
    );

    return instance;
  }

  VkPhysicalDevice pickPhysicalDevice(VkInstance instance)
  {
    uint32_t physicalDeviceCount = 0;
    Assert::vulkan(
      vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr),
      "Failed to enumerate physical devices"
    );

    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    Assert::vulkan(
      vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()),
      "Failed to enumerate physical devices"
    );

    for (uint32_t i = 0; i < physicalDeviceCount; ++i)
    {
      VkPhysicalDeviceProperties props;
      vkGetPhysicalDeviceProperties(physicalDevices[i], &props);

      if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
      {
#ifdef RX_DEBUG
        std::cout << "Vk: GPU: " << props.deviceName << std::endl;
#endif
        return physicalDevices[i];
      }
    }

    // Any non-discrete GPU will not have enough performance to do path tracing at all.
    Error::runtime("No discrete GPU with Vulkan support available", Error::API);
  }

  VkDevice createDevice(VkInstance instance, VkPhysicalDevice physicalDevice, uint32_t* familyIndex)
  {
    VkDevice device;

    *familyIndex = 0;

    uint32_t queueFamilyPropertyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, queueFamilyProperties.data());

    // Iterates over all available queue families and find one that supports the graphics bit.
    for (uint32_t i = 0; i < queueFamilyPropertyCount; ++i)
    {
      if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
      {
        *familyIndex = i;
        break;
      }
    }

    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
    queueInfo.queueFamilyIndex = *familyIndex;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo createInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueInfo;

    std::vector<const char*> extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    Assert::vulkan(
      vkCreateDevice(physicalDevice, &createInfo, nullptr, &device),
      "Failed to create device"
    );

    return device;
  }

  VkSwapchainKHR createSwapChain(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, std::shared_ptr<Window> window, uint32_t* familyIndex)
  {
    VkSwapchainKHR swapChain;

    VkFormatProperties formatProperties = { };
    vkGetPhysicalDeviceFormatProperties(physicalDevice, VK_FORMAT_R8G8B8A8_UNORM, &formatProperties);
    // TODO: check if this color format is supported

    VkSwapchainCreateInfoKHR createInfo = { };
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = 2;
    createInfo.imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
    createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

    int width, height;
    SDL_GetWindowSize(window->getWindow(), &width, &height);
    createInfo.imageExtent.width = static_cast<uint32_t>(width);
    createInfo.imageExtent.height = static_cast<uint32_t>(height);

    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.queueFamilyIndexCount = 1;
    createInfo.pQueueFamilyIndices = familyIndex;
    createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;

    Assert::vulkan(
      vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain),
      "Failed to create swapchain"
    );

    return swapChain;
  }
 
  VkSemaphore createSemaphore(VkDevice device)
  {
    VkSemaphore semaphore;
    
    VkSemaphoreCreateInfo createInfo = { };
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    Assert::vulkan(
      vkCreateSemaphore(device, &createInfo, nullptr, &semaphore),
      "Failed to create semaphore"
    );

    return semaphore;
  }
}