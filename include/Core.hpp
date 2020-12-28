#ifndef CORE_HPP
#define CORE_HPP

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#define VK_ENABLE_BETA_EXTENSIONS
//#define VULKAN_HPP_NO_EXCEPTIONS

#include <external/tinyLogger/TinyLogger.hpp>
#include <external/vkCore/vkCore/vkCore.hpp>

#ifdef RX_BUILD_EXAMPLES
  #undef RX_BUILD_DLL
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

/// A macro to easily replace the renderer's namespace name.
#define RAYEX_NAMESPACE rx

#define RAYEX_NAMESPACE_STRINGIFY_2( text ) #text
#define RAYEX_NAMESPACE_STRINGIFY( text )   RAYEX_NAMESPACE_STRINGIFY_2( text )

#define RAYEX_NAMESPACE_STRINGIFIED RAYEX_NAMESPACE_STRINGIFY( RAYEX_NAMESPACE )

#define RX_VERBOSE( ... )           TINY_LOGGER_NAME::verbose( RAYEX_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )
#define RX_INFO( ... )              TINY_LOGGER_NAME::info( RAYEX_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )
#define RX_SUCCESS( ... )           TINY_LOGGER_NAME::success( RAYEX_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )
#define RX_WARN( ... )              TINY_LOGGER_NAME::warning( RAYEX_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )
#define RX_ERROR( ... )             TINY_LOGGER_NAME::error( RAYEX_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )
#define RX_FATAL( ... )             TINY_LOGGER_NAME::fatal( RAYEX_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )
#define RX_ASSERT( statement, ... ) TINY_LOGGER_NAME::assert2( statement, RAYEX_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )

#define RX_LOG_TIME_START( ... )      \
  std::cout << std::endl;             \
  float startTime = Time::getTime( ); \
  TINY_LOGGER_NAME::info( RAYEX_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )

#define RX_LOG_TIME_STOP( ... ) TINY_LOGGER_NAME::success( RAYEX_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__, " ( Time: ", std::setprecision( std::numeric_limits<float>::digits10 ), Time::getTime( ) - startTime, " s )." )

// Doxygen groups
/// @defgroup BASE Base
/// @defgroup API Vulkan

#endif // CORE_HPP
