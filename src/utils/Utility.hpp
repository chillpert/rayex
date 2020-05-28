#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#define VK_ENABLE_BETA_EXTENSIONS
#include <vulkan/vulkan.h>
#include <iostream>
#include <sstream>

// TODO: rename to RX_LOG
#ifdef RX_DEBUG
  #define RX_DISABLE_LOG Utils::log = false
  #define RX_ENABLE_LOG Utils::log = true

  #define RX_LOG(message) if (Utils::log) std::cout << "RX: Log: " << message << std::endl

  #define RX_ASSERT(statement, message) if (!statement) RX_ERROR(message)

  #define VK_CREATE(result, componentName) if (m_created) RX_ERROR(componentName + " was already created.");\
                                           if (result != VK_SUCCESS) RX_ERROR("Failed to create: " + componentName);\
                                           else RX_LOG("Created: " << componentName);\
                                           m_created = true

  #define VK_DESTROY(term, componentName) if (!m_created) return;\
                                          m_created = false;\
                                          term;\
                                          if (Utils::log) RX_LOG("Destroyed: " << componentName)

  #define VK_FREE(term, componentName) if (!m_created) return;\
                                       m_created = false;\
                                       term;\
                                       if (Utils::log) RX_LOG("Freed: " << componentName)

  #define VK_ASSERT(result, message) if (result != VK_SUCCESS) RX_ERROR(message)

  #define SDL_ASSERT(result, message) if (result != SDL_TRUE)  RX_ERROR(message)

#else
  #define RX_DISABLE_LOG
  #define RX_ENABLE_LOG
  #define RX_ASSERT(statement, message) statement

  #define RX_LOG(message)
  #define VK_CREATE(result, componentName) result
  #define VK_DESTROY(term, componentName) term
  #define VK_FREE(term, componentName) term

  #define VK_ASSERT(result, message) result
  #define SDL_ASSERT(result, message) result
#endif

#define RX_ERROR(message) throw std::runtime_error(std::string("RX: ") + message)

namespace RX
{
  namespace Utils
  {
    extern bool log;
  }
}

#endif // UTILITY_HPP