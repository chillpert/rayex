#include "events/Error.hpp"

#include <exception>

namespace RX
{
  const char* EnumStrings[] = { "Window" , "Api", "General" };

  std::string Error::typeToString(Type type)
  {
    return EnumStrings[type];
  }

  void Error::runtime(const char* message, Type type)
  {
    std::stringstream ss;
    ss << s_label << ": " << typeToString(type) << ": " << message;

    throw std::runtime_error(ss.str());
  }

  void Error::runtime(const std::string& message, Type type)
  {
    std::stringstream ss;
    ss << s_label << ": " << typeToString(type) << ": " << message;

    throw std::runtime_error(ss.str());
  }

  std::string Error::s_label = "RX";
}