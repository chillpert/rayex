#include "api/Instance.hpp"

namespace RX
{
  void Instance::createInstance(std::shared_ptr<Window> window)
  {
#ifdef RX_DEBUG
    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

    bool layersSupported = true;

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
      {
        Error::runtime("Requested validation layers are not available", Error::API);
      }
    }
#endif
    
    // Application info
    VkApplicationInfo appInfo{ };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = window->getProperties().getTitle();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = window->getProperties().getEngineTitle();
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Retrieve all extensions required by SDL2
    uint32_t sdlExtensionsCount = 0;
    window->getInstanceExtensions(sdlExtensionsCount, NULL);

    const char** sdlExtensions = new const char* [sdlExtensionsCount];
    window->getInstanceExtensions(sdlExtensionsCount, sdlExtensions);

    std::vector<const char*> extensions(sdlExtensions, sdlExtensions + sdlExtensionsCount);

    // Instanace create info
    VkInstanceCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

#ifdef RX_DEBUG
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
#else
    createInfo.enabledLayerCount = 0;
#endif

    // check if all required extensions are supported
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensionsProperties(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionsProperties.data());

    // check if all extensions requested by SDL are available on this device
    for (size_t i = 0; i < sdlExtensionsCount; ++i)
    {
      bool found = false;

      for (const auto& extension : extensionsProperties)
      {
        if (strcmp(sdlExtensions[i], extension.extensionName))
        {
          found = true;
          break;
        }
      }

      if (!found)
        Error::runtime("Requested extension not available", Error::API);
    }

    // create instance
    Assert::vulkan(vkCreateInstance(&createInfo, nullptr, &m_instance), "Failed to create Vulkan instance");

    delete[] sdlExtensions;
  }
}