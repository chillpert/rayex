#include "api/vulkan/VulkanApi.hpp"
#include "pch/stdafx.hpp"

#ifdef RX_DEBUG
  const bool enableValidationLayers = true;
#else
  const bool enableValidationLayers = false;
#endif

namespace RX
{
  VulkanApi::VulkanApi()
    : m_validationLayers({ "VK_LAYER_KHRONOS_validation" }) { }

  void VulkanApi::initialize(std::shared_ptr<Window> window)
  {
    Api::initialize(window);

    createInstance();
    m_physicalDeviceManager.findBestDevice(m_instance);
  }

  void VulkanApi::update()
  {
    
  }

  void VulkanApi::render()
  {

  }

  void VulkanApi::clean()
  {
    vkDestroyInstance(m_instance, nullptr);
  }

  void VulkanApi::assertVulkan(VkResult result, const char* message)
  {
    if (result != VK_SUCCESS)
    {
      Error::runtime(message, Error::API);
    }
  }

  void VulkanApi::createInstance()
  {
    // only use validation layers for debug build
    if (enableValidationLayers && !checkValidationLayerSupport())
    {
      Error::runtime("Requested validation layers are not available", Error::API);
    }

    // application info
    VkApplicationInfo appInfo { };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = m_window->getProperties().getTitle();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = m_window->getProperties().getEngineTitle();
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // create info
    VkInstanceCreateInfo createInfo { };
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t sdlExtensionsCount = 0;

    if (!SDL_Vulkan_GetInstanceExtensions(m_window->getWindow(), &sdlExtensionsCount, NULL))
    {
      Error::runtime("SDL failed to get instance extensions", Error::API);
    }

    const char** sdlExtensions = new const char* [sdlExtensionsCount];

    if (!SDL_Vulkan_GetInstanceExtensions(m_window->getWindow(), &sdlExtensionsCount, sdlExtensions))
    {
      Error::runtime("SDL failed to get instance extensions", Error::API);
    }

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (enableValidationLayers)
    {
      createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
      createInfo.ppEnabledLayerNames = m_validationLayers.data();
    }
    else
    {
      createInfo.enabledLayerCount = 0;
    }
    
    if (!checkExtensionSupport(sdlExtensions, sdlExtensionsCount))
    {
      Error::runtime("Requested extension not available", Error::API);
    }
    
    // create instance
    assertVulkan(vkCreateInstance(&createInfo, nullptr, &m_instance), "Failed to create Vulkan instance");

    delete[] sdlExtensions;
  }

  bool VulkanApi::checkExtensionSupport(const char** sdlExtensions, uint32_t sdlExtensionsCount)
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

  bool VulkanApi::checkValidationLayerSupport()
  {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const auto layerName : m_validationLayers)
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

  std::vector<const char*> VulkanApi::getRequiredExtensions()
  {
    uint32_t sdlExtensionsCount = 0;

    if (!SDL_Vulkan_GetInstanceExtensions(m_window->getWindow(), &sdlExtensionsCount, NULL))
    {
      Error::runtime("SDL failed to get instance extensions", Error::API);
    }

    const char** sdlExtensions = new const char* [sdlExtensionsCount];

    if (!SDL_Vulkan_GetInstanceExtensions(m_window->getWindow(), &sdlExtensionsCount, sdlExtensions))
    {
      Error::runtime("SDL failed to get instance extensions", Error::API);
    }

    std::vector<const char*> extensions(sdlExtensions, sdlExtensions + sdlExtensionsCount);

    if (enableValidationLayers)
    {
      extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
  }
}