#ifndef ERROR_HPP
#define ERROR_HPP

#include <sstream>
#include <ostream>

#define VK_ERROR(message) Error::runtime(message, Error::API)

namespace RX
{
  class Error
  {
  public:
    enum Type
    {
      WINDOW,
      API,
      SDL,
      SHADER,
      PIPELINE,
      SWAPCHAIN,
      UNDEFINED
    };

    static std::string typeToString(Type type);

    static void runtime(const char* message, Type type);
    static void runtime(const std::string& message, Type type);
 
  private:
    static std::string s_label;
  };
}

#endif // ERROR_HPP