#include "Instance.hpp"

namespace RX
{
  void Instance::initialize(InstanceInfo& info)
  {
    m_info = info;

    // Retrieve all extensions needed by SDL2.
    std::vector<const char*> windowExtensions = m_info.window->getInstanceExtensions();
    m_info.extensions.insert(m_info.extensions.end(), windowExtensions.begin(), windowExtensions.end());

    // Check if all extensions and layers needed are available.
    checkLayersSupport();
    checkExtensionsSupport();

    // Start creating the instance.
    vk::ApplicationInfo appInfo;
    appInfo.apiVersion = getApiVersion();
 
    vk::InstanceCreateInfo createInfo;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.ppEnabledLayerNames = m_info.layers.data();
    createInfo.enabledLayerCount = static_cast<uint32_t>(m_info.layers.size());
    createInfo.ppEnabledExtensionNames = m_info.extensions.data();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(m_info.extensions.size());

    m_instance = vk::createInstanceUnique(createInfo);

    if (!m_instance)
      RX_ERROR("Failed to create instance.");
  }

  void Instance::checkLayersSupport()
  {
    auto properties = vk::enumerateInstanceLayerProperties();

    for (const char* name : m_info.layers)
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

  void Instance::checkExtensionsSupport()
  {
    auto properties = vk::enumerateInstanceExtensionProperties();

    for (const char* name : m_info.extensions)
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