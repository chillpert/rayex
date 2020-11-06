#pragma once

// A very simple logger written in C++17.
// Tested on both Linux and Windows.
// EXAMPLE
/*
#include "TinyLogger.hpp"

int main( )
{
  TinyLogger::verbose( "Apparently, ", 1, " + ", 2, " = ", 1 + 2 );
  TinyLogger::info( "Ah, okay. Thank you for telling me!" );
  TinyLogger::success( "Are you really telling me it worked?" );
  TinyLogger::warning( "I guess I need to be careful now." );
  TinyLogger::error( "A wild error appeared! Luckily, it's not fatal." );

  try
  {
    TinyLogger::fatal( "Oh my god! I am panicking. I am about to throw a runtime error." );  
  }
  catch( ... )
  {
    TinyLogger::success( "Haha, caught ya!" );
  }  
}
*/

#include <chrono>
#include <iostream>
#include <sstream>
#include <string_view>

#define LOGGER_NAME TinyLogger

#define LOGGER_MESSAGE_TYPE_CAPS_LOCK              false
#define LOGGER_ALIGN_MESSAGES                      true
#define LOGGER_INSERT_SPACE_AFTER_MESSAGE_TYPE     true
#define LOGGER_THROW_RUNTIME_ERROR_FOR_FATAL_ERROR true

class LOGGER_NAME
{
private:
  enum class Color;
  enum class MessageType;

public:
  template <typename... Args>
  static void verbose( Args&&... args )
  {
    std::stringstream temp;
    ( temp << ... << args );

    print( Color::eGray, MessageType::eVerbose, temp.str( ) );
  }

  template <typename... Args>
  static void info( Args&&... args )
  {
    std::stringstream temp;
    ( temp << ... << args );

    print( Color::eWhite, MessageType::eInfo, temp.str( ) );
  }

  template <typename... Args>
  static void success( Args&&... args )
  {
    std::stringstream temp;
    ( temp << ... << args );

    print( Color::eGreen, MessageType::eSuccess, temp.str( ) );
  }

  template <typename... Args>
  static void warning( Args&&... args )
  {
    std::stringstream temp;
    ( temp << ... << args );

    print( Color::eYellow, MessageType::eWarn, temp.str( ) );
  }

  template <typename... Args>
  static void error( Args&&... args )
  {
    std::stringstream temp;
    ( temp << ... << args );

    print( Color::eRed, MessageType::eError, temp.str( ) );
  }

  template <typename... Args>
  static void fatal( Args&&... args )
  {
    std::stringstream temp;
    ( temp << ... << args );

    print( Color::eEmphasizedRed, MessageType::eFatal, temp.str( ) );

    if ( LOGGER_THROW_RUNTIME_ERROR_FOR_FATAL_ERROR )
      throw std::runtime_error( temp.str( ) );
  }

  template <typename... Args>
  static void assert2( bool statement, Args&&... args )
  {
    std::stringstream temp;
    ( temp << ... << args );

    if ( !statement )
    {
      print( Color::eEmphasizedRed, MessageType::eFatal, temp.str( ) );
      throw std::runtime_error( temp.str( ) );
    }
  }

private:
  static std::string_view applyColor( Color color )
  {
    std::string_view colorCode;

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

  static std::string getTime( )
  {
    auto date = std::chrono::system_clock::now( );

    time_t tt        = std::chrono::system_clock::to_time_t( date );
    std::string time = ctime( &tt );
    // Remove the line break inserted by ctime.
    time = time.substr( 0, time.size( ) - 1 );

    std::string result = "[" + time + "] ";

    return result;
  }

  static std::string formatMessageType( MessageType messageType )
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

  static void print( Color color, MessageType messageType, std::string_view message )
  {
    std::stringstream res;
    res << applyColor( color )
        << formatMessageType( messageType )
        << getTime( )
        << message
        << applyColor( Color::eDefault );

    if ( messageType == MessageType::eFatal )
      std::cerr << res.str( ) << std::endl;
    else if ( messageType == MessageType::eVerbose )
      std::clog << res.str( ) << std::endl;
    else
      std::cout << res.str( ) << std::endl;
  }

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

  static const bool capsLock = LOGGER_MESSAGE_TYPE_CAPS_LOCK;
  static const bool align    = LOGGER_ALIGN_MESSAGES;
};
