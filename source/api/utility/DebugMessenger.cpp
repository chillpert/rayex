#include "api/utility/DebugMessenger.hpp"

#include "api/Api.hpp"
#include "api/Components.hpp"

namespace RAYEX_NAMESPACE
{
  DebugMessenger::~DebugMessenger( )
  {
    if ( _debugMessenger )
    {
      destroy( );
    }
  }

  void DebugMessenger::init( vk::DebugUtilsMessageSeverityFlagsEXT messageSeverity, vk::DebugUtilsMessageTypeFlagsEXT messageType )
  {
#ifdef RX_DEBUG
    vk::DebugUtilsMessengerCreateInfoEXT createInfo( { },
                                                     messageSeverity,
                                                     messageType,
                                                     debugMessengerCallback,
                                                     nullptr );

    _debugMessenger = g_instance.createDebugUtilsMessengerEXT( createInfo );
    RX_ASSERT( _debugMessenger, "Failed to create debug messenger." );
#endif
  }

  void DebugMessenger::destroy( )
  {
#ifdef RX_DEBUG
    if ( _debugMessenger )
    {
      g_instance.destroyDebugUtilsMessengerEXT( _debugMessenger );
      _debugMessenger = nullptr;
    }
#endif
  }

  // The source for this function is the official LunarG tutorial.
  VKAPI_ATTR auto VKAPI_CALL debugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
    void* userData ) -> VkBool32
  {
    std::array<char, 64> prefix;
    char* message = static_cast<char*>( malloc( strlen( callbackData->pMessage ) + 500 ) );
    assert( message );

    if ( ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT ) != 0 )
    {
      strcpy( prefix.data( ), "VERBOSE: " );
    }
    else if ( ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT ) != 0 )
    {
      strcpy( prefix.data( ), "INFO: " );
    }
    else if ( ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT ) != 0 )
    {
      strcpy( prefix.data( ), "WARNING: " );
    }
    else if ( ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT ) != 0 )
    {
      strcpy( prefix.data( ), "ERROR: " );
    }

    if ( ( messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT ) != 0U )
    {
      strcat( prefix.data( ), "GENERAL" );
    }
    else
    {
      if ( ( messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT ) != 0U )
      {
        strcat( prefix.data( ), "VALIDATION" );
        if ( ( messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT ) != 0U )
        {
          strcat( prefix.data( ), " | PERFORMANCE" );
        }
      }
    }

    sprintf( message,
             "%s \n\tID Number: %d\n\tID String: %s \n\tMessage:   %s",
             prefix.data( ),
             callbackData->messageIdNumber,
             callbackData->pMessageIdName,
             callbackData->pMessage );

    printf( "%s\n", message );
    fflush( stdout );
    free( message );

    if ( ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT ) != 0 )
    {
      //RX_ERROR("Fatal error");
    }

    return VK_FALSE;
  }
} // namespace RAYEX_NAMESPACE
