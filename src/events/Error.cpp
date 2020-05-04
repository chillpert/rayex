#include "events/Error.hpp"

#include <exception>

namespace RX
{
  std::string Error::typeToString(Type type)
  {
    switch (type)
    {
      case WINDOW:
        return "Window: ";

      case API:
        return "Api: ";

      case UNDEFINED:
        return "General: ";
    }
  }

  void Error::runtime(const char* message, Type type)
  {
    std::stringstream ss;
    ss << s_label << typeToString(type) << message;

    throw std::runtime_error(ss.str());
  }

  void Error::runtime(const std::string& message, Type type)
  {
    std::stringstream ss;
    ss << s_label << typeToString(type) << message;

    throw std::runtime_error(ss.str());
  }

  std::string Error::s_label = "RX: ";
}