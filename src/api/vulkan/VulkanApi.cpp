#include "api/vulkan/VulkanApi.hpp"
#include "pch/stdafx.hpp"

#ifdef RX_DEBUG
  const bool enableValidationLayers = true;
#else
  const bool enableValidationLayers = false;
#endif

namespace RX
{
  const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

  void VulkanApi::initialize(std::shared_ptr<Window> window)
  {
    Api::initialize(window);

    createInstance();
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

    createInfo.enabledExtensionCount = sdlExtensionsCount;
    createInfo.ppEnabledExtensionNames = sdlExtensions;
    createInfo.enabledLayerCount = 0;

    
    // create instance
    assertVulkan(vkCreateInstance(&createInfo, nullptr, &m_instance), "Failed to create Vulkan instance");
  
    // checking for extension support
    /*
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::cout << extensionCount << std::endl;

    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "Available extensions:\n";

    for (const auto& extension : extensions) {
      std::cout << '\t' << extension.extensionName << '\n';
    }
    */

    delete[] sdlExtensions;
  }
}