#include "events/Assert.hpp"

namespace RX
{
  void Assert::vulkan(VkResult result, const char* message)
  {
    if (result != VK_SUCCESS)
    {
      Error::runtime(message, Error::API);
    }
  }

  void Assert::sdl(SDL_bool result, const char* message)
  {
    if (result != SDL_TRUE)
    {
      Error::runtime(message, Error::SDL);
    }
  }
}