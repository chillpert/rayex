#ifndef VK_DEBUG_MESSENGER_HPP
#define VK_DEBUG_MESSENGER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class DebugMessenger
  {
  public:
    void initialize(VkInstance instance);
    void destroy(VkInstance instance);

  private:
    PFN_vkCreateDebugUtilsMessengerEXT m_createDebugUtilsMessengerEXT;
    PFN_vkDestroyDebugUtilsMessengerEXT m_destroyDebugUtilsMessengerEXT;
    VkDebugUtilsMessengerEXT m_debugMessenger;
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