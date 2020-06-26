#ifndef VK_DEBUG_MESSENGER_HPP
#define VK_DEBUG_MESSENGER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct DebugMessengerInfo
  {
    VkInstance instance;
    VkDebugUtilsMessageSeverityFlagsEXT messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    VkDebugUtilsMessageTypeFlagsEXT messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
  };

  class DebugMessenger
  {
  public:
    ~DebugMessenger();

    inline VkDebugUtilsMessengerEXT get() { return m_debugMessenger; }

    void init(DebugMessengerInfo& info);

  private:
    void destroy();
    
    PFN_vkCreateDebugUtilsMessengerEXT m_createDebugUtilsMessengerEXT;
    PFN_vkDestroyDebugUtilsMessengerEXT m_destroyDebugUtilsMessengerEXT;

    VkDebugUtilsMessengerEXT m_debugMessenger;
    DebugMessengerInfo m_info;
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