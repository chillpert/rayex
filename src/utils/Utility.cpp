#include "Utility.hpp"

namespace RX
{
  namespace Utils
  {
    bool log = true;

    void setLogging(bool state)
    {
      log = state;
    }

    VkResult assertVulkan(VkResult result, const char* message)
    {
      if (result != VK_SUCCESS)
        RX_ERROR(message);

      return result;
    }

    void assertSdl(SDL_bool result, const char* message)
    {
      if (result != SDL_TRUE)
      {
        RX_ERROR(message);
      }
    }
  }
}