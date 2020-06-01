#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#define VK_ENABLE_BETA_EXTENSIONS
#include <vulkan/vulkan.h>
#include <iostream>
#include <sstream>
#include <iomanip>

//#define NO_UTILS

#ifndef NO_UTILS
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

  #define VK_ALLOCATE(result, componentName) if (m_allocated)\
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
                                             m_allocated = true

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

  #define VK_FREE(term, componentName) if (!m_allocated)\
                                       {\
                                         RX_LOG(componentName << " was not allocated and cannot be freed.");\
                                         RX_ERROR(componentName + " was not allocated and cannot be freed.");\
                                         return;\
                                       }\
                                       m_allocated = false;\
                                       term;\
                                       Utils::freeCounter += 1;\
                                       if (Utils::log) RX_LOG("Freed:     " << componentName)

  #define VK_ASSERT(result, message) if (result != VK_SUCCESS) throw std::runtime_error(std::string("RX: ") + message)

  #define SDL_ASSERT(result, message) if (result != SDL_TRUE)  RX_ERROR(message)

  #define RX_EMPTY_LINE std::cout << std::endl

  #define RX_SEPARATOR "==================================================================="
  #define RX_FORMAT_INFO(message) RX_LOG(message << "\n" << RX_SEPARATOR)

  #define RX_FORMAT_TABLE2(message, column1, column2, term) RX_LOG(message << "\n" << RX_SEPARATOR << "\n" << column1 << "\t\t\t" << column2);\
                                                            std::cout << RX_SEPARATOR << "\n";\
                                                            term;\
                                                            std::cout << std::endl

  #define RX_FORMAT_TABLE2_ENTRY(entry1, entry2) std::cout << std::left << std::setw(32) << std::setfill(' ') << entry1 << std::left << std::setw(32) << std::setfill(' ') << entry2

  #define RX_DEBUG_ONLY(term) term
  #define RX_PRINT(message) std::cout << message << std::endl
  #define RX_PRINT_CONTINOUS(message) std::cout << message
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

  #define RX_EMPTY_LINE
  #define RX_FORMAT_INFO(message, term)
  #define RX_FORMAT_TABLE2(message, column1, column2, term)
  #define RX_FORMAT_TABLE2_ENTRY(entry1, entry2)

  #define RX_DEBUG_ONLY(term)
  #define RX_PRINT(message)
  #define RX_PRINT_CONTINOUS(message)
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