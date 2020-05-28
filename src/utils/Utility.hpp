#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#define VK_ENABLE_BETA_EXTENSIONS
#include <vulkan/vulkan.h>
#include <iostream>
#include <sstream>

// TODO: rename to RX_LOG
#ifdef RX_DEBUG
  #define RX_DISABLE_LOG Utils::setLogging(false)
  #define RX_ENABLE_LOG Utils::setLogging(true)  

  #define VK_LOG(message) if (Utils::log) std::cout << "RX: Log: " << message << std::endl

  #define VK_ASSERT(result, message) Utils::assertVulkan(result, message)
  #define SDL_ASSERT(result, message) Utils::assertSdl(result, message)
#else
  #define RX_DISABLE_LOG
  #define RX_ENABLE_LOG 

  #define VK_LOG(message)

  #define VK_ASSERT(result, message) result
  #define SDL_ASSERT(result, message) result
#endif

#define RX_ERROR(message) throw std::runtime_error(std::string("RX: ") + message)

namespace RX
{
  namespace Utils
  {
    extern bool log;

    void setLogging(bool state);

    VkResult assertVulkan(VkResult result, const char* message);
    void assertSdl(SDL_bool result, const char* message);

    struct StringBuilder
    {
      std::stringstream ss;

      template<typename T>
      StringBuilder& operator<<(const T& data)
      {
        ss << data;
        return *this;
      }

      operator std::string() { return ss.str(); }
    };
  }
}

#endif // UTILITY_HPP