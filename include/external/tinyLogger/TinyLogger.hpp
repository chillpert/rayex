#ifndef LOGGER_HPP
#define LOGGER_HPP

// A very simple logger written in C++.
// Include this header to your project, but do not forget to compile the source file.
// C++17 is required.

// EXAMPLE
/*
#include "TinyLogger.hpp"

int main( )
{
  tinyLogger::verbose( "Apparently, ", 1, " + ", 2, " = ", 1 + 2 );
  tinyLogger::info( "Ah, okay. Thank you for telling me!" );
  tinyLogger::success( "Are you really telling me it worked?" );
  tinyLogger::warning( "I guess I need to be careful now." );
  tinyLogger::error( "A wild error appeared! Luckily, it's not fatal." );

  try
  {
    tinyLogger::fatal( "Oh my god! I am panicking. I am about to throw a runtime error." );  
  }
  catch( ... )
  {
    tinyLogger::success( "Haha, caught ya!" );
  }  
}
*/

#include <sstream>

#define LOGGER_NAMESPACE tinyLogger

#define LOGGER_MESSAGE_TYPE_CAPS_LOCK              false
#define LOGGER_ALIGN_MESSAGES                      true
#define LOGGER_INSERT_SPACE_AFTER_MESSAGE_TYPE     true
#define LOGGER_THROW_RUNTIME_ERROR_FOR_FATAL_ERROR true

#if defined( _WIN32 ) || defined( _WIN64 ) || defined( unix ) || defined( __unix ) || defined( __unix__ )
#else
  #error "Operating system not supported by logger."
#endif

#if defined( _WIN32 ) || defined( _WIN64 )
  #ifdef RX_BUILD_DLL
    #define RX_API __declspec( dllexport )
  #else
    #define RX_API __declspec( dllimport )
  #endif
#elif defined( unix ) || defined( __unix ) || defined( __unix__ )
  #ifdef RX_BUILD_DLL
    #define RX_API __attribute__( ( visibility( "default" ) ) )
  #else
    #define RX_API
  #endif
#endif

namespace LOGGER_NAMESPACE
{
  enum class Color
  {
    eGray,
    eWhite,
    eGreen,
    eYellow,
    eRed,
    eEmphasizedRed,
    eDefault
  };

  enum class MessageType
  {
    eVerbose,
    eInfo,
    eWarn,
    eSuccess,
    eError,
    eFatal
  };

  RX_API void print( Color color, MessageType messageType, std::string_view message );

  template <typename... Args>
  void verbose( Args&&... args )
  {
    std::stringstream temp;
    ( temp << ... << args );

    print( Color::eGray, MessageType::eVerbose, temp.str( ) );
  }

  template <typename... Args>
  void info( Args&&... args )
  {
    std::stringstream temp;
    ( temp << ... << args );

    print( Color::eWhite, MessageType::eInfo, temp.str( ) );
  }

  template <typename... Args>
  void success( Args&&... args )
  {
    std::stringstream temp;
    ( temp << ... << args );

    print( Color::eGreen, MessageType::eSuccess, temp.str( ) );
  }

  template <typename... Args>
  void warning( Args&&... args )
  {
    std::stringstream temp;
    ( temp << ... << args );

    print( Color::eYellow, MessageType::eWarn, temp.str( ) );
  }

  template <typename... Args>
  void error( Args&&... args )
  {
    std::stringstream temp;
    ( temp << ... << args );

    print( Color::eRed, MessageType::eError, temp.str( ) );
  }

  template <typename... Args>
  void fatal( Args&&... args )
  {
    std::stringstream temp;
    ( temp << ... << args );

    print( Color::eEmphasizedRed, MessageType::eFatal, temp.str( ) );

    if ( LOGGER_THROW_RUNTIME_ERROR_FOR_FATAL_ERROR )
      throw std::runtime_error( temp.str( ) );
  }

  template <typename... Args>
  void assert2( bool statement, Args&&... args )
  {
    std::stringstream temp;
    ( temp << ... << args );

    if ( !statement )
    {
      print( Color::eEmphasizedRed, MessageType::eFatal, temp.str( ) );
      throw std::runtime_error( temp.str( ) );
    }
  }
} // namespace LOGGER_NAMESPACE

#endif // LOGGER_HPP