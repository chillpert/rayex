#include "utils/Error.hpp"

#include <exception>
#include <iostream>

namespace RX
{
  std::string Error::s_label = "RX";
  
  const char* EnumStrings[] = { "Window" , "Api", "Sdl", "Shader", "Pipeline", "Swapchain", "General" };

  std::string Error::typeToString(Type type)
  {
    return EnumStrings[type];
  }

  void Error::runtime(const char* message, Type type)
  {
    std::stringstream ss;
    ss << s_label << ": " << typeToString(type) << ": " << message;

    std::cout << ss.str() << std::endl;

    throw std::runtime_error(ss.str());
  }

  void Error::runtime(const std::string& message, Type type)
  {
    std::stringstream ss;
    ss << s_label << ": " << typeToString(type) << ": " << message;

    std::cout << ss.str() << std::endl;

    throw std::runtime_error(ss.str());
  }
}