#include "Instance.hpp"
#include "Components.hpp"

namespace rx
{
  Instance::Instance(const std::vector<const char*>& layers, std::vector<const char*>& extensions, bool initialize)
  {
    if (initialize)
      init(layers, extensions);
  }

  void Instance::init(const std::vector<const char*>& layers, std::vector<const char*>& extensions)
  {
    // Retrieve all extensions needed by SDL2.
    std::vector<const char*> windowExtensions = g_window->getInstanceExtensions();
    extensions.insert(extensions.end(), windowExtensions.begin(), windowExtensions.end());

    // Check if all extensions and layers needed are available.
    checkLayersSupport(layers);
    checkExtensionsSupport(extensions);

    // Start creating the instance.
    vk::ApplicationInfo appInfo;
    appInfo.apiVersion = getApiVersion();
 
    vk::InstanceCreateInfo createInfo{
      { },                                      // flags
      &appInfo,                                 // pApplicationInfo
      static_cast<uint32_t>(layers.size()),     // enabledLayerCount
      layers.data(),                            // ppEnabledLayerNames
      static_cast<uint32_t>(extensions.size()), // enabledExtensionCount
      extensions.data()                         // ppEnabledExtensionNames
    };

    m_instance = vk::createInstanceUnique(createInfo);
    g_instance = m_instance.get();

    if (!m_instance)
      RX_ERROR("Failed to create instance.");

    g_dispatchLoaderDynamic = std::make_unique<vk::DispatchLoaderDynamic>(g_instance, vkGetInstanceProcAddr);
  }

  void Instance::checkLayersSupport(const std::vector<const char*>& layers)
  {
    auto properties = vk::enumerateInstanceLayerProperties();

    for (const char* name : layers)
    {
      bool found = false;
      for (const auto& property : properties)
      {
        if (strcmp(property.layerName, name) == 0)
        {
          found = true;
          break;
        }
      }

      if (!found)
        RX_ERROR("Validation layer " + name + " is not available on this device");

      RX_LOG("Added layer: " << name);
    }
  }

  void Instance::checkExtensionsSupport(const std::vector<const char*>& extensions)
  {
    auto properties = vk::enumerateInstanceExtensionProperties();

    for (const char* name : extensions)
    {
      bool found = false;
      for (const auto& property : properties)
      {
        if (strcmp(property.extensionName, name) == 0)
        {
          found = true;
          break;
        }
      }

      if (!found)
        RX_ERROR("Instance extensions " + name + " is not available on this device");

      RX_LOG("Added extension: " << name);
    }
  }

  uint32_t Instance::getApiVersion()
  {
    uint32_t apiVersion = vk::enumerateInstanceVersion();

#ifdef VK_API_VERSION_1_2
    if (apiVersion >= VK_API_VERSION_1_2)
    {
      RX_LOG("Found Vulkan SDK API Version 1.2.x");
      return VK_API_VERSION_1_2;
    }
#endif

#ifdef VK_API_VERSION_1_1
    if (apiVersion >= VK_API_VERSION_1_1)
    {
      RX_LOG("Found Vulkan SDK API Version 1.1.x");
      return VK_API_VERSION_1_1;
    }
#endif
    
    RX_ERROR("This application requires Vulkan SDK API Version 1.1 or higher.");
  
    return 0;
  }
}