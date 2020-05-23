cmake_minimum_required(VERSION 3.17.2)
project(Renderer VERSION 0.0.1)

message("Using compiler: " + ${CMAKE_CXX_COMPILER_ID})

option(BUILD_EXAMPLES "Build examples" ON)

if(NOT CMAKE_BUILD_TYPE)
  message("MX: Build type not set. Continuing with: Debug")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
      "No build type was set"
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

add_compile_definitions(RX_BUILD_DLL)

if(WIN32 OR MSVC)
  set(OS ${OS} "Windows")
  add_compile_definitions(RX_PLATFORM_WINDOWS_X64)
  message("Renderer: Target platform Windows (x64)")
elseif(UNIX)
  set(OS ${OS} "Linux")
  add_compile_definitions(RX_PLATFORM_UNIX_X64)
  message("Renderer: Target platform Windows (x64)")
else()
  message(FATAL_ERROR "Renderer: Target platform is not supported")
endif()

set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/external/Cotire")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(RX_DEBUG)
endif()

# RESOURCES
add_compile_definitions(RX_PROJECT_PATH="${PROJECT_SOURCE_DIR}/")
add_compile_definitions(RX_RESOURCES_PATH="${PROJECT_SOURCE_DIR}/resources/")
add_compile_definitions(RX_SHADER_PATH="${PROJECT_SOURCE_DIR}/resources/shaders/")

# VULKAN
find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

if(${OS} STREQUAL "Windows")
  add_compile_definitions(RX_GLSLC_PATH="${Vulkan_INCLUDE_DIRS}/../Bin/glslc.exe")
elseif(${OS} STREQUAL "Linux")
  add_compile_definitions(RX_GLSLC_PATH="${PROJECT_SOURCE_DIR}/external/Vulkan/glslc")
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

# HEADER PROJECT FILES
include_directories(${PROJECT_SOURCE_DIR}/external/glm/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

# SOURCE PROJECT FILES
file(GLOB FILES_LIB
  "${PROJECT_SOURCE_DIR}/src/api/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/api/swapchain/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/api/buffers/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/api/devices/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/api/sync/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/window/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/pch/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/events/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/utils/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

# LIBRARY
add_library(renderer SHARED ${FILES_LIB})

target_link_libraries(renderer
  ${WINDOWING}
  ${Vulkan_LIBRARY}
)

target_precompile_headers(renderer
  PUBLIC "${PROJECT_SOURCE_DIR}/src/pch/stdafx.hpp"
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

# EXAMPLE
if(BUILD_EXAMPLES)
  include_directories(${PROJECT_SOURCE_DIR}/src/example)

  file(GLOB FILES_EXAMPLE
    "${PROJECT_SOURCE_DIR}/example/*.cpp"
  )

  add_executable(example ${PROJECT_SOURCE_DIR}/src/example/main.cpp ${FILES_EXAMPLE})

  link_directories(${PROJECT_SOURCE_DIR}/build/bin/renderer/debug)

  target_link_libraries(example
    renderer
  )

  set_target_properties(example
    PROPERTIES
    CXX_STANDARD 17
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
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_PATH debug)
  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(BUILD_PATH release)
  elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(BUILD_PATH relWithDebInfo) 
  elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(BUILD_PATH minSizeRel)
  endif()

  # copy dlls to binary location in post build
  if(${OS} STREQUAL "Windows")
    add_custom_command(
      TARGET example
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
              ${PROJECT_SOURCE_DIR}/external/SDL2/binaries/win64/SDL2.dll
              ${PROJECT_SOURCE_DIR}/build/bin/${BUILD_PATH}/SDL2.dll
      COMMENT "Running POST_BUILD action: Copying SDL2d.dll to binary location"
      )
  endif()
endif()
