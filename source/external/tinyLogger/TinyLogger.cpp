#include "external/tinyLogger/TinyLogger.hpp"

#include <chrono>
#include <iostream>

#if defined( _WIN32 ) || defined( _WIN64 )
  #include <Windows.h>
HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
#endif

namespace LOGGER_NAMESPACE
{
  const bool capsLock = LOGGER_MESSAGE_TYPE_CAPS_LOCK;
  const bool align    = LOGGER_ALIGN_MESSAGES;

  std::string applyColorLinux( Color color )
  {
    std::string colorCode;

    switch ( color )
    {
      case Color::eGray:
        colorCode = "\033[0;90m";
        break;

      case Color::eWhite:
        colorCode = "\033[0;37m";
        break;

      case Color::eGreen:
        colorCode = "\033[0;32m";
        break;

      case Color::eYellow:
        colorCode = "\033[0;33m";
        break;

      case Color::eRed:
        colorCode = "\033[0;31m";
        break;

      case Color::eEmphasizedRed:
        colorCode = "\033[1;4;31m";
        break;

      case Color::eDefault:
        colorCode = "\033[0;37m";
        break;
    }

    return colorCode;
  }

#if defined( _WIN32 ) || defined( _WIN64 )
  void applyColorWindows( Color color )
  {
    switch ( color )
    {
      case Color::eGray:
        SetConsoleTextAttribute( hConsole, 8 );
        break;

      case Color::eWhite:
        SetConsoleTextAttribute( hConsole, 15 );
        break;

      case Color::eGreen:
        SetConsoleTextAttribute( hConsole, 10 );
        break;

      case Color::eYellow:
        SetConsoleTextAttribute( hConsole, 14 );
        break;

      case Color::eRed:
        SetConsoleTextAttribute( hConsole, 4 );
        break;

      case Color::eEmphasizedRed:
        SetConsoleTextAttribute( hConsole, 12 );
        break;

      case Color::eDefault:
        SetConsoleTextAttribute( hConsole, 15 );
        break;
    }
  }
#endif

  std::string getTime( )
  {
    auto date = std::chrono::system_clock::now( );

    time_t tt        = std::chrono::system_clock::to_time_t( date );
    std::string time = ctime( &tt );
    // Remove the line break inserted by ctime.
    time = time.substr( 0, time.size( ) - 1 );

    std::string result = "[" + time + "] ";

    return result;
  }

  std::string formatMessageType( MessageType messageType )
  {
    std::string result;

    switch ( messageType )
    {
      case MessageType::eVerbose:
        capsLock ? result = "VERBOSE" : result = "Verbose";
        break;

      case MessageType::eInfo:
        capsLock ? result = "INFO" : result = "Info";
        if ( align ) result += "   ";
        break;

      case MessageType::eWarn:
        capsLock ? result = "WARNING" : result = "Warning";
        break;

      case MessageType::eSuccess:
        capsLock ? result = "SUCCESS" : result = "Success";
        break;

      case MessageType::eError:
        capsLock ? result = "ERROR" : result = "Error";
        if ( align ) result += "  ";
        break;

      case MessageType::eFatal:
        capsLock ? result = "FATAL" : result = "Fatal";
        if ( align ) result += "  ";
        break;
    }

    if ( LOGGER_INSERT_SPACE_AFTER_MESSAGE_TYPE )
      result += " ";

    return result;
  }

  void print( Color color, MessageType messageType, std::string_view message )
  {
#if defined( _WIN32 ) || defined( _WIN64 )
    std::stringstream res;
    res << formatMessageType( messageType )
        << getTime( )
        << message;

    // Apply color.
    applyColorWindows( color );

    if ( messageType == MessageType::eFatal )
      std::cerr << res.str( ) << std::endl;
    else if ( messageType == MessageType::eVerbose )
      std::clog << res.str( ) << std::endl;
    else
      std::cout << res.str( ) << std::endl;

    // Reset color.
    applyColorWindows( Color::eDefault );

#elif defined( unix ) || defined( __unix ) || defined( __unix__ )

    std::stringstream res;
    res << applyColorLinux( color )
        << formatMessageType( messageType )
        << getTime( )
        << message
        << applyColorLinux( Color::eDefault );

    if ( messageType == MessageType::eFatal )
      std::cerr << res.str( ) << std::endl;
    else if ( messageType == MessageType::eVerbose )
      std::clog << res.str( ) << std::endl;
    else
      std::cout << res.str( ) << std::endl;

#else
  #error "Operating system not supported by logger."
#endif
  }
} // namespace LOGGER_NAMESPACE
