#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#define VK_ENABLE_BETA_EXTENSIONS
#include <vulkan/vulkan.h>
#include <iostream>
#include <sstream>

#ifdef RX_DEBUG
  #define VK_LOG(message) std::cout << "RX: Log: " << message << std::endl

  #define VK_ASSERT(result, message) Utils::assertVulkan(result, message)
  #define SDL_ASSERT(result, message) Utils::assertSdl(result, message)
#else
  #define VK_LOG(message)

  #define VK_ASSERT(result, message) result
  #define SDL_ASSERT(result, message) result
#endif

#define RX_ERROR(message) Utils::throwRuntimeError(message)

namespace RX
{
  namespace Utils
  {
    void throwRuntimeError(const char* message);
    void throwRuntimeError(const std::string& message);

    void assertVulkan(VkResult result, const char* message);
    void assertSdl(SDL_bool result, const char* message);
  }
}

#endif // UTILITY_HPP