#ifndef VK_DEBUG_MESSENGER_HPP
#define VK_DEBUG_MESSENGER_HPP

#include "pch/stdafx.hpp"

namespace rx
{
  // TODO: Use C++ Wrapper instead of native API.
  class DebugMessenger
  {
  public:
    DebugMessenger() = default;
    DebugMessenger(VkDebugUtilsMessageSeverityFlagsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, bool initialize = true);

    // Scope-bound destruction.
    ~DebugMessenger();

    /*
      Creates the debug messenger with the given properties.
      @param messageSeverity - Only messages that match the provided flag bits will be printed to the console.
      @param messageType - Only messages that match the provided flag bits will be printed to the console.
    */
    void init(VkDebugUtilsMessageSeverityFlagsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType);

  private:
    void destroy();
    
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