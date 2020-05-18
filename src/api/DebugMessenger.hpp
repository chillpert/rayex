#ifndef VK_DEBUG_MESSENGER_HPP
#define VK_DEBUG_MESSENGER_HPP

#include "BaseComponent.hpp"

namespace RX
{
  class DebugMessenger : public BaseComponent
  {
  public:
    DebugMessenger();
    ~DebugMessenger();

    void initialize(VkInstance instance);

  private:
    void destroy();
    
    PFN_vkCreateDebugUtilsMessengerEXT m_createDebugUtilsMessengerEXT;
    PFN_vkDestroyDebugUtilsMessengerEXT m_destroyDebugUtilsMessengerEXT;
    VkDebugUtilsMessengerEXT m_debugMessenger;

    VkInstance m_instance;
  };

  VKAPI_ATTR VkBool32 VKAPI_CALL debugMessengerCallback
  (
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
    void* userData
  );
}

#endif // VK_DEBUG_MESSENGER_HPP