#ifndef CORE_HPP
#define CORE_HPP

#include "external/tinyLogger/TinyLogger.hpp"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#define VK_ENABLE_BETA_EXTENSIONS
//#define VULKAN_HPP_NO_EXCEPTIONS
#include <iostream>
#include <vulkan/vulkan.hpp>

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
#define RAYEXEC_NAMESPACE rx

#define RAYEXEC_NAMESPACE_STRINGIFY_2( text ) #text
#define RAYEXEC_NAMESPACE_STRINGIFY( text )   RAYEXEC_NAMESPACE_STRINGIFY_2( text )

#define RAYEXEC_NAMESPACE_STRINGIFIED RAYEXEC_NAMESPACE_STRINGIFY( RAYEXEC_NAMESPACE )

#define RX_VERBOSE( ... ) LOGGER_NAMESPACE::verbose( RAYEXEC_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )
#define RX_INFO( ... )    LOGGER_NAMESPACE::info( RAYEXEC_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )
#define RX_SUCCESS( ... ) LOGGER_NAMESPACE::success( RAYEXEC_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )
#define RX_WARN( ... )    LOGGER_NAMESPACE::warning( RAYEXEC_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )
#define RX_ERROR( ... )   LOGGER_NAMESPACE::error( RAYEXEC_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )
#define RX_FATAL( ... )   LOGGER_NAMESPACE::fatal( RAYEXEC_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )
// Using redundant boolean literal in ternary expression to support vulkan object error check.
#define RX_ASSERT( statement, ... ) LOGGER_NAMESPACE::assert2( statement ? true : false, RAYEXEC_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )

// Doxygen groups
/// @defgroup Base Interfaces
/// @defgroup API Vulkan

#endif // CORE_HPP