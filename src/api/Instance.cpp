#include "Instance.hpp"

namespace RX
{
  Instance::~Instance()
  {
    destroy();
  }

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
    VkApplicationInfo appInfo{ };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = getApiVersion();
 
    VkInstanceCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.ppEnabledLayerNames = m_info.layers.data();
    createInfo.enabledLayerCount = static_cast<uint32_t>(m_info.layers.size());
    createInfo.ppEnabledExtensionNames = m_info.extensions.data();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(m_info.extensions.size());

    VK_CREATE(vkCreateInstance(&createInfo, nullptr, &m_instance), "instance.");
  }

  void Instance::destroy()
  {
    VK_DESTROY(vkDestroyInstance(m_instance, nullptr), "instance");
  }

  void Instance::checkLayersSupport()
  {
    uint32_t propertyCount;
    VK_ASSERT(vkEnumerateInstanceLayerProperties(&propertyCount, nullptr), "Failed to enumerate instance layer properties.");

    std::vector<VkLayerProperties> properties(propertyCount);
    VK_ASSERT(vkEnumerateInstanceLayerProperties(&propertyCount, properties.data()), "Failed to enumerate instance layer properties.");

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
    uint32_t propertyCount;
    VK_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr), "Failed to enumerate instance extension properties.");

    std::vector<VkExtensionProperties> properties(propertyCount);
    VK_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, properties.data()), "Failed to enumerate instance extension properties.");

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
    uint32_t apiVersion;
    VK_ASSERT(vkEnumerateInstanceVersion(&apiVersion), "Failed to enumerate instance version.");

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