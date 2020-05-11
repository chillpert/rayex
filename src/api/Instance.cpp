#include "Instance.hpp"

namespace RX
{
  void Instance::pushLayer(const char* name)
  {
    checkLayerSupport(name);

    layers.push_back(name);
    VK_LOG("Added Layer: " << name);
  }

  void Instance::pushExtension(const char* name)
  {
    checkExtensionSupport(name);

    extensions.push_back(name);
    VK_LOG("Added Extension: " << name);
  }

  void Instance::create(const std::shared_ptr<Window> const window)
  {
    VkApplicationInfo appInfo{ };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = getApiVersion();

    VkInstanceCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

#ifdef RX_DEBUG
    pushLayer("VK_LAYER_KHRONOS_validation");

    createInfo.ppEnabledLayerNames = layers.data();
    createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
#endif

    // Retrieves all extensions needed by SDL2
    uint32_t sdlExtensionsCount;
    window->getInstanceExtensions(sdlExtensionsCount, NULL);

    const char** sdlExtensionsNames = new const char* [sdlExtensionsCount];

    window->getInstanceExtensions(sdlExtensionsCount, sdlExtensionsNames);

    for (size_t i = 0; i < sdlExtensionsCount; ++i)
    {
      pushExtension(sdlExtensionsNames[i]);
    }

#ifdef RX_DEBUG
    pushExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());

    VK_ASSERT(vkCreateInstance(&createInfo, nullptr, &instance), "Failed to create instance");
  }

  void Instance::checkLayerSupport(const char* name)
  {
    uint32_t propertyCount;
    VK_ASSERT(vkEnumerateInstanceLayerProperties(&propertyCount, nullptr), "Failed to enumerate instance layer properties");

    std::vector<VkLayerProperties> properties(propertyCount);
    VK_ASSERT(vkEnumerateInstanceLayerProperties(&propertyCount, properties.data()), "Failed to enumerate instance layer properties");

    for (const auto& property : properties)
    {
      if (strcmp(property.layerName, name))
        return;
    }
    
    VK_ERROR("Validation layer is not available on this device");
  }

  void Instance::checkExtensionSupport(const char* name)
  {
    uint32_t propertyCount;
    VK_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr), "Failed to enumerate instance extension properties");

    std::vector<VkExtensionProperties> properties(propertyCount);
    VK_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, properties.data()), "Failed to enumerate instance extension properties");

    for (const auto& property : properties)
    {
      if (strcmp(property.extensionName, name))
        return;
    }

    VK_ERROR("Validation layer is not available on this device");
  }

  uint32_t Instance::getApiVersion()
  {
    uint32_t apiVersion;
    VK_ASSERT(vkEnumerateInstanceVersion(&apiVersion), "Failed to enumerate instance version");

    if (apiVersion >= VK_API_VERSION_1_2)
      VK_LOG("Found Vulkan SDK API Version 1.2");

    else if (apiVersion >= VK_API_VERSION_1_1)
      VK_LOG("Found Vulkan SDK API Version 1.1");

    else
      VK_ERROR("The Vulkan SDK on this device is outdated");
  
    return apiVersion;
  }
}