#include "DebugMessenger.hpp"
#include "Api.hpp"
#include "Components.hpp"

namespace RX
{
  DebugMessenger::DebugMessenger(VkDebugUtilsMessageSeverityFlagsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, bool initialize)
  {
    if (initialize)
      init(messageSeverity, messageType);
  }

  DebugMessenger::~DebugMessenger()
  {
    if (m_debugMessenger)
      destroy();
  }

  void DebugMessenger::init(VkDebugUtilsMessageSeverityFlagsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType)
  {
#ifdef RX_DEBUG
    m_createDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(g_instance, "vkCreateDebugUtilsMessengerEXT");
    m_destroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(g_instance, "vkDestroyDebugUtilsMessengerEXT");

    VkDebugUtilsMessengerCreateInfoEXT createInfo{
      .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
      .pNext = nullptr,
      .flags = 0,
      .messageSeverity = messageSeverity,
      .messageType = messageType,
      .pfnUserCallback = debugMessengerCallback,
      .pUserData = nullptr
    };
    
    m_createDebugUtilsMessengerEXT(g_instance, &createInfo, nullptr, &m_debugMessenger);
#endif
  }

  void DebugMessenger::destroy()
  {
#ifdef RX_DEBUG
    m_destroyDebugUtilsMessengerEXT(g_instance, m_debugMessenger, nullptr);
    m_debugMessenger = VK_NULL_HANDLE;
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
    char* message = static_cast<char*>(malloc(strlen(callbackData->pMessage) + 500));
    assert(message);

    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
    {
      strcpy(prefix, "VERBOSE: ");
    }
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
    {
      strcpy(prefix, "INFO: ");
    }
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
      strcpy(prefix, "WARNING: ");
    }
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
      strcpy(prefix, "ERROR: ");
    }

    if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
    {
      strcat(prefix, "GENERAL");
    }
    else
    {
      if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
      {
        strcat(prefix, "VALIDATION");
        if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
        {
          strcat(prefix, " | PERFORMANCE");
        }
      }
    }

    sprintf
    (
      message,
      "%s \n\tID Number: %d\n\tID String: %s \n\tMessage:   %s",
      prefix,
      callbackData->messageIdNumber,
      callbackData->pMessageIdName,
      callbackData->pMessage
    );

    printf("%s\n", message);
    fflush(stdout);
    free(message);

    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
      //RX_ERROR("Fatal error");
    }

    return VK_FALSE;
  }
}