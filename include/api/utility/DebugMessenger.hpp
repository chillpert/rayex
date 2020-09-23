#ifndef DEBUG_MESSENGER_HPP
#define DEBUG_MESSENGER_HPP

#include "pch/stdafx.hpp"

namespace RENDERER_NAMESPACE
{
  /// A wrapper class for a Vulkan debug utility messenger.
  /// @ingroup API
  class DebugMessenger
  {
  public:
    DebugMessenger( ) = default;
    
    /// Calls destroy().
    ~DebugMessenger( );

    /// Creates the debug messenger with the given properties.
    /// @param messageSeverity - Specifies the type of severity of messages that will be logged.
    /// @param messageType - Specifies the types of messages that will be logged.
    /// @return Returns true if initialization was successful.
    bool init( vk::DebugUtilsMessageSeverityFlagsEXT messageSeverity, vk::DebugUtilsMessageTypeFlagsEXT messageType );

  private:
    /// Destroys the debug messenger.
    void destroy( );

    vk::DebugUtilsMessengerEXT debugMessenger;
  };

  /// @cond INTERNAL
  VKAPI_ATTR VkBool32 VKAPI_CALL debugMessengerCallback( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                         VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                         const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
                                                         void* userData );
  /// @endcond
}

#endif // DEBUG_MESSENGER_HPP