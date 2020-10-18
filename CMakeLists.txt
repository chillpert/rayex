cmake_minimum_required(VERSION 3.16.2)
project(Renderer VERSION 0.0.1)

message("Using compiler: " + ${CMAKE_CXX_COMPILER_ID})

option(BUILD_EXAMPLES "Build examples" ON)
option(CLANG_TIDY_LINUX_DEBUG "Use clang-tidy for a Linux debug build" OFF)

if(NOT CMAKE_BUILD_TYPE)
  message("MX: Build type not set. Continuing with: Debug")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "No build type was set" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

message(${CMAKE_BUILD_TYPE})

if(WIN32 OR MSVC)
  set(OS ${OS} "Windows")
  message("Renderer: Target platform Windows (x64)")
elseif(UNIX)
  set(OS ${OS} "Linux")
  message("Renderer: Target platform Linux (x64)")
else()
  message(FATAL_ERROR "Renderer: Target platform is not supported")
endif()

set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/external/Cotire")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(RX_DEBUG)
endif()

if(BUILD_EXAMPLES)
  add_compile_definitions(RX_ASSETS_PATH="${PROJECT_SOURCE_DIR}/assets/")
else()
  add_compile_definitions(RX_ASSETS_PATH="")
endif()

# VULKAN
find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

if(${OS} STREQUAL "Windows")
  add_compile_definitions(RX_GLSLC_PATH="${Vulkan_INCLUDE_DIRS}/../Bin/glslc.exe")
elseif(${OS} STREQUAL "Linux")
  add_compile_definitions(RX_GLSLC_PATH="/usr/bin/glslc")
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CLANG_TIDY_LINUX_DEBUG)
      set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")
    endif()
  endif()
endif()

# SDL
if(${OS} STREQUAL "Windows")
  link_directories(${PROJECT_SOURCE_DIR}/external/SDL2/binaries/win64)
  include_directories(${PROJECT_SOURCE_DIR}/external/SDL2/include)

  set(WINDOWING ${WINDOWING} "SDL2main.lib" "SDL2.lib")
elseif(${OS} STREQUAL "Linux")
  find_package(SDL2 REQUIRED)
  include_directories(${SDL2_INCLUDE_DIRS})

  set(WINDOWING ${WINDOWING} "${SDL2_LIBRARIES}")
else()
  message(FATAL_ERROR, "Target platform not supported")
endif()

include_directories(${PROJECT_SOURCE_DIR}/external/STB)
include_directories(${PROJECT_SOURCE_DIR}/external/ModelLoading)

include(${PROJECT_SOURCE_DIR}/include/CMakeLists.txt)
include(${PROJECT_SOURCE_DIR}/source/CMakeLists.txt)

# LIBRARY
add_library(renderer SHARED ${FILES_LIB})

set_target_properties(renderer PROPERTIES COMPILE_DEFINITIONS "RX_BUILD_DLL")

target_link_libraries(renderer
  ${WINDOWING}
  ${Vulkan_LIBRARY}
)

target_precompile_headers(renderer
  PUBLIC "${PROJECT_SOURCE_DIR}/include/pch/stdafx.hpp"
)

set_target_properties(renderer PROPERTIES
  CXX_STANDARD 17
)

set_target_properties(renderer
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/build/lib/debug"
  LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/build/lib/debug"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/build/bin/debug"

  ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/build/lib/release"
  LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/build/lib/release"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/build/bin/release"

  ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${PROJECT_SOURCE_DIR}/build/lib/minsizerel"
  LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${PROJECT_SOURCE_DIR}/build/lib/minsizerel"
  RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${PROJECT_SOURCE_DIR}/build/bin/minsizerel"   

  ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/build/lib/relwithbebinfo"
  LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/build/lib/relwithbebinfo"
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/build/bin/relwithbebinfo"
)

# set build paths
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  set(BUILD_PATH debug)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  set(BUILD_PATH release)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
  set(BUILD_PATH relWithDebInfo) 
elseif(${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
  set(BUILD_PATH minSizeRel)
endif()

# EXAMPLE
if(BUILD_EXAMPLES)
  include(${PROJECT_SOURCE_DIR}/example/CMakeLists.txt)
endif()

add_compile_definitions(RX_PATH_TO_LIBRARY="${PROJECT_SOURCE_DIR}/build/bin/${BUILD_PATH}/")
