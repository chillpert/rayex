#ifndef CORE_HPP
#define CORE_HPP

#include "Logger.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#define VK_ENABLE_BETA_EXTENSIONS
#include <vulkan/vulkan.hpp>
#include <iostream>

#define RX_SHADER_GROUP_INDEX_RGEN 0
#define RX_SHADER_GROUP_INDEX_MISS 1
#define RX_SHADER_GROUP_INDEX_CHIT 2

//#define NO_UTILS

#ifndef NO_UTILS
#define RX_LOG(message) std::cout << "rx: Log: " << message << std::endl
#define RX_ASSERT(statement, message) if (!statement) throw std::runtime_error(std::string("rx: ") + message)
#define RX_ERROR(message) throw std::runtime_error(std::string("rx: ") + message)
#else
#define RX_LOG(message)
#define RX_ASSERT(statement, message) statement
#define RX_ERROR(message)
#endif

#ifdef RX_PLATFORM_WINDOWS_X64
#ifdef RX_BUILD_DLL
#define RX_API __declspec(dllexport)
#else
#define RX_API __declspec(dllimport)
#endif
#elif RX_PLATFORM_UNIX_X64
#ifdef RX_BUILD_DLL
#define RX_API __attribute__((visibility("default")))
#else   
#define RX_API
#endif
#endif

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

// Doxygen groups
/// @defgroup Base Interfaces
/// @defgroup API Vulkan

#endif // CORE_HPP