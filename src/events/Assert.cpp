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
}