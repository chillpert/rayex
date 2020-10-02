#include "api/utility/DebugMessenger.hpp"
#include "api/Api.hpp"
#include "api/misc/Components.hpp"

namespace RAYEXEC_NAMESPACE
{
  DebugMessenger::~DebugMessenger( )
  {
    if ( this->debugMessenger )
      destroy( );
  }

  void DebugMessenger::init( vk::DebugUtilsMessageSeverityFlagsEXT messageSeverity, vk::DebugUtilsMessageTypeFlagsEXT messageType )
  {
  #ifdef RX_DEBUG
    vk::DebugUtilsMessengerCreateInfoEXT createInfo( { },
                                                     messageSeverity,
                                                     messageType,
                                                     debugMessengerCallback,
                                                     nullptr );

    this->debugMessenger = g_instance.createDebugUtilsMessengerEXT( createInfo );
    RX_ASSERT( this->debugMessenger, "Failed to create debug messenger." );
  #endif
  }

  void DebugMessenger::destroy( )
  {
  #ifdef RX_DEBUG
    if ( this->debugMessenger )
    {
      g_instance.destroyDebugUtilsMessengerEXT( this->debugMessenger );
      this->debugMessenger = nullptr;
    }
  #endif
  }

  // The source for this function is the official LunarG tutorial.
  VKAPI_ATTR VkBool32 VKAPI_CALL debugMessengerCallback
  (
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
    void* userData
  )
  {
    char prefix[64];
    char* message = static_cast<char*>( malloc( strlen( callbackData->pMessage ) + 500 ) );
    assert( message );

    if ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT )
    {
      strcpy( prefix, "VERBOSE: " );
    }
    else if ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT )
    {
      strcpy( prefix, "INFO: " );
    }
    else if ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT )
    {
      strcpy( prefix, "WARNING: " );
    }
    else if ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT )
    {
      strcpy( prefix, "ERROR: " );
    }

    if ( messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT )
    {
      strcat( prefix, "GENERAL" );
    }
    else
    {
      if ( messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT )
      {
        strcat( prefix, "VALIDATION" );
        if ( messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT )
        {
          strcat( prefix, " | PERFORMANCE" );
        }
      }
    }

    sprintf( message,
             "%s \n\tID Number: %d\n\tID String: %s \n\tMessage:   %s",
             prefix,
             callbackData->messageIdNumber,
             callbackData->pMessageIdName,
             callbackData->pMessage );

    printf( "%s\n", message );
    fflush( stdout );
    free( message );

    if ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT )
    {
      //RX_ERROR("Fatal error");
    }

    return VK_FALSE;
  }
}