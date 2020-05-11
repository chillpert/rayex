#include "Instance.hpp"

namespace RX
{
  void Instance::pushLayer(const char* name)
  {
    checkLayerSupport(name);

    m_layers.push_back(name);
  }
  void Instance::pushExtension(const char* name)
  {
    checkExtensionSupport(name);

    m_extensions.push_back(name);
  }

  void Instance::create(const std::shared_ptr<Window> const window)
  {
    VkApplicationInfo appInfo{ };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

#ifdef RX_DEBUG
    pushLayer("VK_LAYER_KHRONOS_validation");

    createInfo.ppEnabledLayerNames = m_layers.data();
    createInfo.enabledLayerCount = static_cast<uint32_t>(m_layers.size());
#endif

    // Retrieves all extensions needed by SDL2
    uint32_t sdlExtensionsCount;
    window->getInstanceExtensions(sdlExtensionsCount, NULL);

    const char** sdlExtensionsNames = new const char* [sdlExtensionsCount];
    window->getInstanceExtensions(sdlExtensionsCount, sdlExtensionsNames);

    m_extensions = std::vector<const char*>(sdlExtensionsNames, sdlExtensionsNames + sdlExtensionsCount);

#ifdef RX_DEBUG
    pushExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    createInfo.ppEnabledExtensionNames = m_extensions.data();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(m_extensions.size());

    VK_ASSERT(vkCreateInstance(&createInfo, nullptr, &m_instance), "Failed to create instance");
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
    VK_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr), "Failed to enumerate instance layer properties");

    std::vector<VkExtensionProperties> properties(propertyCount);
    VK_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, properties.data()), "Failed to enumerate instance layer properties");

    for (const auto& property : properties)
    {
      if (strcmp(property.extensionName, name))
        return;
    }

    VK_ERROR("Validation layer is not available on this device");
  }
}