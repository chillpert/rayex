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

  #define RX_ASSERT(statement, message) if (!statement) throw std::runtime_error(std::string("RX: ") + message)

  #define VK_CREATE(result, componentName) if (m_created)\
                                           {\
                                             RX_LOG(componentName << " was already created.");\
                                             RX_ERROR(componentName + " was already created.");\
                                           }\
                                           Utils::createCounter += 1;\
                                           if (result != VK_SUCCESS)\
                                           {\
                                             RX_ERROR("Failed to create: " + componentName);\
                                           }\
                                           else RX_LOG("Created:   " << componentName);\
                                           m_created = true

  #define VK_ALLOCATE(result, componentName) if (m_created)\
                                             {\
                                               RX_LOG(componentName << " was already allocated.");\
                                               RX_ERROR(componentName + " was already allocated.");\
                                             }\
                                             Utils::allocateCounter += 1;\
                                             if (result != VK_SUCCESS)\
                                             {\
                                               RX_ERROR("Failed to allocate: " + componentName);\
                                             }\
                                             else RX_LOG("Allocated: " << componentName);\
                                             m_created = true

  #define VK_DESTROY(term, componentName) if (!m_created)\
                                          {\
                                            RX_LOG(componentName << " was not created and cannot be destroyed");\
                                            RX_ERROR(componentName + " was not created and cannot be destroyed");\
                                            return;\
                                          }\
                                          m_created = false;\
                                          term;\
                                          Utils::destroyCounter += 1;\
                                          if (Utils::log) RX_LOG("Destroyed: " << componentName)

  #define VK_FREE(term, componentName) if (!m_created)\
                                       {\
                                         RX_LOG(componentName << " was not allocated and cannot be freed.");\
                                         RX_ERROR(componentName + " was not allocated and cannot be freed.");\
                                         return;\
                                       }\
                                       m_created = false;\
                                       term;\
                                       Utils::freeCounter += 1;\
                                       if (Utils::log) RX_LOG("Freed:     " << componentName)

  #define VK_ASSERT(result, message) if (result != VK_SUCCESS) throw std::runtime_error(std::string("RX: ") + message)

  #define SDL_ASSERT(result, message) if (result != SDL_TRUE)  RX_ERROR(message)

#else
  #define RX_DISABLE_LOG
  #define RX_ENABLE_LOG
  #define RX_ASSERT(statement, message) statement

  #define RX_LOG(message)
  #define VK_CREATE(result, componentName) result
  #define VK_ALLOCATE(result, componentName) result
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
    extern size_t createCounter;
    extern size_t destroyCounter;

    extern size_t allocateCounter;
    extern size_t freeCounter;
  }
}

#endif // UTILITY_HPP