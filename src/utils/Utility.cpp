#include "Utility.hpp"

namespace RX
{
  namespace Utils
  {
    void throwRuntimeError(const char* message)
    {
      std::stringstream ss;
      ss << "RX: " << message;

      std::cout << ss.str() << std::endl;

      throw std::runtime_error(ss.str());
    }

    void throwRuntimeError(const std::string& message)
    {
      std::stringstream ss;
      ss << "RX: " << message;

      std::cout << ss.str() << std::endl;

      throw std::runtime_error(ss.str());
    }

    void assertVulkan(VkResult result, const char* message)
    {
      if (result != VK_SUCCESS)
        throwRuntimeError(message);
    }

    void assertSdl(SDL_bool result, const char* message)
    {
      if (result != SDL_TRUE)
      {
        throwRuntimeError(message);
      }
    }
  }
}