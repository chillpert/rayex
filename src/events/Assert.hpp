#ifndef ASSERT_HPP
#define ASSERT_HPP

#include "events/Error.hpp"

#include <vulkan/vulkan.h>

namespace RX
{
  class Assert
  {
  public:
    static void vulkan(VkResult result, const char* message);
  };
}

#endif // ASSERT_HPP