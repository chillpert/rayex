#ifndef ASSERT_HPP
#define ASSERT_HPP

#include "events/Error.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>

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