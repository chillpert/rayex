#ifndef VK_MESSENGER_HPP
#define VK_MESSENGER_HPP

#ifdef RX_DEBUG
#include "pch/stdafx.hpp"

namespace RX
{
  class VkMessenger
  {
  public:
    void create(VkInstance instance);
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

#endif // RX_DEBUG

#endif // VK_MESSENGER_HPP