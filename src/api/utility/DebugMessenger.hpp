#ifndef VK_DEBUG_MESSENGER_HPP
#define VK_DEBUG_MESSENGER_HPP

#include "pch/stdafx.hpp"

namespace rx
{
  /// A wrapper class for a Vulkan debug utility messenger.
  /// @ingroup API
  class DebugMessenger
  {
  public:
    DebugMessenger( ) = default;
    
    /// @param messageSeverity Specifies the type of severity of messages that will be logged.
    /// @param messageType Specifies the types of messages that will be logged.
    /// @param initialize If true, the debug messenger will be initialized right away without an additional call to init().
    DebugMessenger( vk::DebugUtilsMessageSeverityFlagsEXT messageSeverity, vk::DebugUtilsMessageTypeFlagsEXT messageType, bool initialize = true );

    /// Calls destroy().
    ~DebugMessenger( );

    /// Creates the debug messenger with the given properties.
    /// @param messageSeverity - Specifies the type of severity of messages that will be logged.
    /// @param messageType - Specifies the types of messages that will be logged.
    void init( vk::DebugUtilsMessageSeverityFlagsEXT messageSeverity, vk::DebugUtilsMessageTypeFlagsEXT messageType );

  private:
    /// Destroys the debug messenger.
    void destroy( );

    vk::DebugUtilsMessengerEXT m_debugMessenger;
  };

  /// @cond INTERNAL
  VKAPI_ATTR VkBool32 VKAPI_CALL debugMessengerCallback( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                         VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                         const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
                                                         void* userData );
  /// @endcond
}

#endif // VK_DEBUG_MESSENGER_HPP