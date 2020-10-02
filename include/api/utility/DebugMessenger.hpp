#ifndef DEBUG_MESSENGER_HPP
#define DEBUG_MESSENGER_HPP

#include "pch/stdafx.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// A wrapper class for a Vulkan debug utility messenger.
  ///
  /// The class features scope-bound destruction.
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
    void init( vk::DebugUtilsMessageSeverityFlagsEXT messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError, vk::DebugUtilsMessageTypeFlagsEXT messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation );

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
} // namespace RAYEXEC_NAMESPACE

#endif // DEBUG_MESSENGER_HPP