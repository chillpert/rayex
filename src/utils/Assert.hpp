#ifndef ASSERT_HPP
#define ASSERT_HPP

#include "utils/Error.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#define VK_ENABLE_BETA_EXTENSIONS
#include <vulkan/vulkan.h>

#ifdef RX_DEBUG
  #define VK_LOG(message) std::cout << "Vk: Log: " << message << std::endl
  #define VK_ASSERT(result, message) Assert::vulkan(result, message)
#else
  #define VK_LOG(message)
  #define VK_ASSERT(result, message) result
#endif

namespace RX
{
  class Assert
  {
  public:
    static void vulkan(VkResult result, const char* message);
    static void sdl(SDL_bool result, const char* message);
  };
}

#endif // ASSERT_HPP