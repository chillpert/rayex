#ifndef CORE_HPP
#define CORE_HPP

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

#endif // CORE_HPP