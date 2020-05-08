#include "api/Instance.hpp"

namespace RX
{
  const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

  void Instance::createInstance(Window* window)
  {
#ifdef RX_DEBUG
    // only use validation layers for debug build
    if (!checkValidationLayerSupport())
    {
      Error::runtime("Requested validation layers are not available", Error::API);
    }
#endif

    // application info
    VkApplicationInfo appInfo{ };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = window->getProperties().getTitle();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = window->getProperties().getEngineTitle();
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // create info
    VkInstanceCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t sdlExtensionsCount = 0;

    if (!SDL_Vulkan_GetInstanceExtensions(window->getWindow(), &sdlExtensionsCount, NULL))
    {
      Error::runtime("SDL failed to get instance extensions", Error::API);
    }

    const char** sdlExtensions = new const char* [sdlExtensionsCount];

    if (!SDL_Vulkan_GetInstanceExtensions(window->getWindow(), &sdlExtensionsCount, sdlExtensions))
    {
      Error::runtime("SDL failed to get instance extensions", Error::API);
    }

    auto extensions = getRequiredExtensions(window);
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

#ifdef RX_DEBUG
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
#else
    createInfo.enabledLayerCount = 0;
#endif

    if (!checkExtensionSupport(sdlExtensions, sdlExtensionsCount))
    {
      Error::runtime("Requested extension not available", Error::API);
    }

    // create instance
    Assert::vulkan(vkCreateInstance(&createInfo, nullptr, &m_instance), "Failed to create Vulkan instance");

    delete[] sdlExtensions;
  }

  void Instance::destroyInstance()
  {
    vkDestroyInstance(m_instance, nullptr);
  }

  bool Instance::checkExtensionSupport(const char** sdlExtensions, uint32_t sdlExtensionsCount)
  {
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    // check if all extensions requested by SDL are available on this device
    for (size_t i = 0; i < sdlExtensionsCount; ++i)
    {
      bool found = false;

      for (const auto& extension : extensions)
      {
        if (strcmp(sdlExtensions[i], extension.extensionName))
        {
          found = true;
          break;
        }
      }

      if (!found)
        return false;

    }

    return true;
  }

  bool Instance::checkValidationLayerSupport()
  {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const auto layerName : validationLayers)
    {
      bool found = false;

      for (const auto& layerProperties : availableLayers)
      {
        if (strcmp(layerName, layerProperties.layerName) == 0)
        {
          found = true;
          break;
        }
      }

      if (!found)
        return false;
    }

    return true;
  }

  std::vector<const char*> Instance::getRequiredExtensions(Window* window)
  {
    uint32_t sdlExtensionsCount = 0;

    if (!SDL_Vulkan_GetInstanceExtensions(window->getWindow(), &sdlExtensionsCount, NULL))
    {
      Error::runtime("SDL failed to get instance extensions", Error::API);
    }

    const char** sdlExtensions = new const char* [sdlExtensionsCount];

    if (!SDL_Vulkan_GetInstanceExtensions(window->getWindow(), &sdlExtensionsCount, sdlExtensions))
    {
      Error::runtime("SDL failed to get instance extensions", Error::API);
    }

    std::vector<const char*> extensions(sdlExtensions, sdlExtensions + sdlExtensionsCount);

#ifdef RX_DEBUG
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    return extensions;
  }
}