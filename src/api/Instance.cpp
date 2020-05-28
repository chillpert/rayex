#include "Instance.hpp"

namespace RX
{
  Instance::Instance(VkInstance* instance) :
    m_instance(instance) { }

  Instance::~Instance()
  {
    destroy();
  }

  void Instance::initialize(InstanceInfo& info)
  {
    // Retrieve all extensions needed by SDL2.
    std::vector<const char*> windowExtensions = info.window->getInstanceExtensions();
    info.extensions.insert(info.extensions.end(), windowExtensions.begin(), windowExtensions.end());

    // Check if all extensions and layers needed are available.
    checkLayersSupport(info.layers);
    checkExtensionsSupport(info.extensions);

    // Start creating the instance.
    VkApplicationInfo appInfo{ };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = getApiVersion();
 
    VkInstanceCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.ppEnabledLayerNames = info.layers.data();
    createInfo.enabledLayerCount = static_cast<uint32_t>(info.layers.size());    
    createInfo.ppEnabledExtensionNames = info.extensions.data();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(info.extensions.size());

    VK_ASSERT(vkCreateInstance(&createInfo, nullptr, m_instance), "Failed to create instance.");
  }

  void Instance::destroy()
  {
    vkDestroyInstance(*m_instance, nullptr);
  }

  void Instance::print()
  {
    uint32_t layerCount;
    VK_ASSERT(vkEnumerateInstanceLayerProperties(&layerCount, nullptr), "Failed to enumerate instance layer properties.");

    std::vector<VkLayerProperties> layers(layerCount);
    VK_ASSERT(vkEnumerateInstanceLayerProperties(&layerCount, layers.data()), "Failed to enumerate instance layer properties.");
    
    VK_LOG("\n\nAvailable layers on this device:");
    std::cout << "==================================================================\n";
    for (const auto& layer : layers)
      std::cout << "  " << layer.layerName << ":\n\t" << "Description: " << layer.description << std::endl;
    std::cout << std::endl;

    uint32_t extensionCount;
    VK_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr), "Failed to enumerate instance extension properties.");

    std::vector<VkExtensionProperties> extensions(extensionCount);
    VK_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data()), "Failed to enumerate instance extension properties.");

    VK_LOG("\n\nAvailable extensions on this device:");
    std::cout << "==================================================================\n";
    for (const auto& extension : extensions)
      std::cout << "  " << extension.extensionName << std::endl;
    std::cout << std::endl;
  }

  void Instance::checkLayersSupport(const std::vector<const char*>& layers)
  {
    uint32_t propertyCount;
    VK_ASSERT(vkEnumerateInstanceLayerProperties(&propertyCount, nullptr), "Failed to enumerate instance layer properties.");

    std::vector<VkLayerProperties> properties(propertyCount);
    VK_ASSERT(vkEnumerateInstanceLayerProperties(&propertyCount, properties.data()), "Failed to enumerate instance layer properties.");

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

      VK_LOG("Added layer: " << name);
    }
  }

  void Instance::checkExtensionsSupport(const std::vector<const char*>& extensions)
  {
    uint32_t propertyCount;
    VK_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr), "Failed to enumerate instance extension properties.");

    std::vector<VkExtensionProperties> properties(propertyCount);
    VK_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, properties.data()), "Failed to enumerate instance extension properties.");

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

      VK_LOG("Added extension: " << name);
    }
  }

  uint32_t Instance::getApiVersion()
  {
    uint32_t apiVersion;
    VK_ASSERT(vkEnumerateInstanceVersion(&apiVersion), "Failed to enumerate instance version.");

#ifdef VK_API_VERSION_1_2
    if (apiVersion >= VK_API_VERSION_1_2)
    {
      VK_LOG("Found Vulkan SDK API Version 1.2.x");
      return VK_API_VERSION_1_2;
    }
#endif

#ifdef VK_API_VERSION_1_1
    if (apiVersion >= VK_API_VERSION_1_1)
    {
      VK_LOG("Found Vulkan SDK API Version 1.1.x");
      return VK_API_VERSION_1_1;
    }
#endif
    
    RX_ERROR("This application requires Vulkan SDK API Version 1.1 or higher.");
  
    return 0;
  }
}