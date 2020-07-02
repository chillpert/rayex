#ifndef VK_DEBUG_MESSENGER_HPP
#define VK_DEBUG_MESSENGER_HPP

#include "pch/stdafx.hpp"

namespace rx
{
  class DebugMessenger
  {
  public:
    DebugMessenger( ) = default;
    DebugMessenger( vk::DebugUtilsMessageSeverityFlagsEXT messageSeverity, vk::DebugUtilsMessageTypeFlagsEXT messageType, bool initialize = true );

    // Scope-bound destruction.
    ~DebugMessenger( );

    // Creates the debug messenger with the given properties.
    // @param messageSeverity - Only messages that match the provided flag bits will be printed to the console.
    // @param messageType - Only messages that match the provided flag bits will be printed to the console.
    void init( vk::DebugUtilsMessageSeverityFlagsEXT messageSeverity, vk::DebugUtilsMessageTypeFlagsEXT messageType );

  private:
    void destroy( );

    vk::DebugUtilsMessengerEXT m_debugMessenger;
  };

  VKAPI_ATTR VkBool32 VKAPI_CALL debugMessengerCallback( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                         VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                         const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
                                                         void* userData );
}

#endif // VK_DEBUG_MESSENGER_HPP