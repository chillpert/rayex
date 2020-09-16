#ifndef LOGGER_HPP
#define LOGGER_HPP

// A very simple logger written in C++.
// Include this header to your project, but do not forget to compile the source file. 
// C++17 is required.

// EXAMPLE
/*
#include "Logger.hpp"

int main( )
{
  logger::verbose( "Apparently, ", 1, " + ", 2, " = ", 1 + 2 );
  logger::info( "Ah, okay. Thank you for telling me!" );
  logger::success( "Are you really telling me it worked?" );
  logger::warning( "I guess I need to be careful now." );
  logger::error( "A wild error appeared! Luckily, it's not fatal." );

  try
  {
    logger::fatal( "Oh my god! I am panicking. I am about to throw a runtime error." );  
  }
  catch( ... )
  {
    logger::success( "Haha, caught ya!" );
  }  
}
*/

#include <sstream>

#define LOGGER_NAMESPACE logger
#define LOGGER_DLL_EXPORT
#define LOGGER_DLL_BUILD
#define LOGGER_MESSAGE_TYPE_CAPS_LOCK false
#define LOGGER_ALIGN_MESSAGES true
#define LOGGER_INSERT_SPACE_AFTER_MESSAGE_TYPE true
#define LOGGER_THROW_RUNTIME_ERROR_FOR_FATAL_ERROR true

#ifdef LOGGER_DLL_EXPORT
  #if defined( _WIN32 ) || defined( _WIN64 )
    #ifdef LOGGER_DLL_BUILD
      #define DLL_EXPORT __declspec(dllexport)
    #else
      #define RX_API __declspec(dllimport)
    #endif
  #elif defined( unix ) || defined( __unix ) || defined( __unix__ )
    #define DLL_EXPORT __attribute__((visibility("default")))
  #else
    #error "Operating system not supported by logger."
  #endif
#else
  #if defined( _WIN32 ) || defined( _WIN64 )
    #define DLL_EXPORT
  #elif defined( unix ) || defined( __unix ) || defined( __unix__ )
    #define DLL_EXPORT
  #else
    #error "Operating system not supported by logger."
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

  void print( Color color, MessageType messageType, const std::string& message );

  template <typename ...Args>
  DLL_EXPORT void verbose( Args&& ...args )
  {
    std::stringstream temp;
    ( temp << ... << args );

    print( Color::eGray, MessageType::eVerbose, temp.str( ) );
  }

  template <typename ...Args>
  DLL_EXPORT void info( Args&& ...args )
  {
    std::stringstream temp;
    ( temp << ... << args );

    print( Color::eWhite, MessageType::eInfo, temp.str( ) );
  }

  template <typename ...Args>
  DLL_EXPORT void success( Args&& ...args )
  {
    std::stringstream temp;
    ( temp << ... << args );

    print( Color::eGreen, MessageType::eSuccess, temp.str( ) );
  }

  template <typename ...Args>
  DLL_EXPORT void warning( Args&& ...args )
  {
    std::stringstream temp;
    ( temp << ... << args );

    print( Color::eYellow, MessageType::eWarn, temp.str( ) );
  }

  template <typename ...Args>
  DLL_EXPORT void error( Args&& ...args )
  {
    std::stringstream temp;
    ( temp << ... << args );

    print( Color::eRed, MessageType::eError, temp.str( ) );
  }

  template <typename ...Args>
  DLL_EXPORT void fatal( Args&& ...args )
  {
    std::stringstream temp;
    ( temp << ... << args );

    print( Color::eEmphasizedRed, MessageType::eFatal, temp.str( ) );

    if ( LOGGER_THROW_RUNTIME_ERROR_FOR_FATAL_ERROR )
      throw std::runtime_error( temp.str( ) );
  }
}

#endif // LOGGER_HPP