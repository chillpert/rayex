#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#define VK_ENABLE_BETA_EXTENSIONS
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>

//#define NO_UTILS

#ifndef NO_UTILS
  #define RX_DISABLE_LOG Utils::log = false
  #define RX_ENABLE_LOG Utils::log = true

  #define RX_LOG(message) if (Utils::log) std::cout << "RX: Log: " << message << std::endl

  #define RX_ASSERT(statement, message) if (!statement) throw std::runtime_error(std::string("RX: ") + message)

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
  }
}

#endif // UTILITY_HPP