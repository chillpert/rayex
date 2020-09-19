#ifndef CORE_HPP
#define CORE_HPP

#include "external/tinyLogger/TinyLogger.hpp"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#define VK_ENABLE_BETA_EXTENSIONS
#include <vulkan/vulkan.hpp>
#include <iostream>

#define RX_SHADER_GROUP_INDEX_RGEN 0
#define RX_SHADER_GROUP_INDEX_MISS 1
#define RX_SHADER_GROUP_INDEX_CHIT 2

#ifdef RX_BUILD_EXAMPLES
#undef RX_BUILD_DLL
#endif

#if defined( _WIN32 ) || defined( _WIN64 )
#ifdef RX_BUILD_DLL
#define RX_API __declspec(dllexport)
#else
#define RX_API __declspec(dllimport)
#endif
#elif defined( unix ) || defined( __unix ) || defined( __unix__ )
#ifdef RX_BUILD_DLL
#define RX_API __attribute__((visibility("default")))
#else   
#define RX_API
#endif
#endif

/// A macro to easily replace the renderer's namespace name.
#define RENDERER_NAMESPACE rx

#define RENDERER_NAMESPACE_STRINGIFY_2( text ) #text
#define RENDERER_NAMESPACE_STRINGIFY( text )  RENDERER_NAMESPACE_STRINGIFY_2( text )

#define RENDERER_NAMESPACE_STRINGIFIED RENDERER_NAMESPACE_STRINGIFY( RENDERER_NAMESPACE )

#define RX_VERBOSE( ... ) LOGGER_NAMESPACE::verbose( RENDERER_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )
#define RX_INFO( ... )    LOGGER_NAMESPACE::info( RENDERER_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )
#define RX_SUCCESS( ... ) LOGGER_NAMESPACE::success( RENDERER_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )
#define RX_WARN( ... )    LOGGER_NAMESPACE::warning( RENDERER_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )
#define RX_ERROR( ... )   LOGGER_NAMESPACE::error( RENDERER_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )
#define RX_FATAL( ... )   LOGGER_NAMESPACE::fatal( RENDERER_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )
#define RX_ASSERT( statement, ... ) LOGGER_NAMESPACE::assert2( statement ? true : false, RENDERER_NAMESPACE_STRINGIFIED, ": ", __VA_ARGS__ )

// Doxygen groups
/// @defgroup Base Interfaces
/// @defgroup API Vulkan

#endif // CORE_HPP