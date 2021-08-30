cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME "Rayex")
project(${PROJECT_NAME} VERSION 0.1.0)

# Notes: 
# - change build type by calling '$ cmake -DCMAKE_BUILD_TYPE=<some_build_type> ..'
# - do not forget about the different build options below

message("===================== ${PROJECT_NAME} ======================")

# Check operating system compatibility
if(NOT((${CMAKE_SYSTEM_NAME} STREQUAL "Windows") OR (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")))
  message(FATAL_ERROR "Target platform is not supported. Rayex only supports Microsoft Windows and GNU/Linux.")
endif()

# Build options
option(CLANG_TIDY_LINUX_DEBUG "Use clang-tidy for a Linux debug build" OFF)
option(VARIANCE_CALCULATOR "Variance can be estimated at a high performance penalty." OFF)

# In case no build type was specified continue with debug build
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "No build type was set" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# Print some information about the system and Rayex's build options
message("OS:                              " ${CMAKE_SYSTEM_NAME})
message("Using compiler:                  " ${CMAKE_CXX_COMPILER_ID})
message("Compiler version:                " ${CMAKE_CXX_COMPILER_VERSION})
message("Build type:                      " ${CMAKE_BUILD_TYPE})
message("Clang Tidy:                      " ${CLANG_TIDY_LINUX_DEBUG})
message("Experimental variance estimator: " ${VARIANCE_CALCULATOR} "\n")

message("==================== Starting ====================")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  # TODO: modernize
  # Use with clang tidy
  if(CLANG_TIDY_LINUX_DEBUG)
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")
  endif()
endif()

# Include 'include helper'
include(${PROJECT_SOURCE_DIR}/include/CMakeLists.txt)

# SDL
#add_subdirectory(${PROJECT_SOURCE_DIR}/include/external/SDL ${PROJECT_SOURCE_DIR}/include/external/SDL/build)
#set(SDL2_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include/external/SDL/include)
#set(SDL2_LIBRARIES "libSDL2d.a")

# Sources to compile
file(GLOB RAYEX_SOURCES
  "${PROJECT_SOURCE_DIR}/source/api/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/api/pathtrace/*.cpp"

  "${PROJECT_SOURCE_DIR}/source/pch/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/events/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/utils/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/base/*.cpp"

  "${PROJECT_SOURCE_DIR}/include/external/imgui/*.cpp"
  "${PROJECT_SOURCE_DIR}/include/external/imgui/backends/imgui_impl_sdl.cpp"
  "${PROJECT_SOURCE_DIR}/include/external/imgui/backends/imgui_impl_vulkan.cpp"
)

# Rayex (library)
add_library(rayex 
  SHARED
    ${RAYEX_SOURCES} 
)

target_compile_definitions(rayex 
  PRIVATE
    $<$<BOOL:${VARIANCE_CALCULATOR}>:RX_VARIANCE_ESTIMATOR>
    
    # Validation layers and error checking is only active in debug builds
    $<$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>:RX_DEBUG>

    # Tell compiler about location of glslc
    RX_GLSLC_PATH="${Vulkan_GLSLC_EXECUTABLE}"

    RX_BUILD_DLL
)

# Include directories for Rayex
target_include_directories(rayex 
  PUBLIC
    ${Vulkan_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
)

# Configure the PCH
target_precompile_headers(rayex
  PUBLIC 
    "${PROJECT_SOURCE_DIR}/include/pch/stdafx.hpp"
)

# Configure output directories for all build types
set_target_properties(rayex PROPERTIES
  CXX_STANDARD 20

  ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build"
  LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build"
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build"
)

target_link_directories(rayex 
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include/external/libs
)

target_link_libraries(rayex
  PUBLIC
    ${SDL2_LIBRARIES}
    ${Vulkan_LIBRARY}
    ${KTX}
)
