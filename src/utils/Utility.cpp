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

    VkResult assertVulkan(VkResult result, const char* message)
    {
      if (result != VK_SUCCESS)
        throwRuntimeError(message);

      return result;
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