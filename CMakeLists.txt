cmake_minimum_required(VERSION 3.14.0)
project(Renderer VERSION 0.0.1)

option(BUILD_EXAMPLE "Build example" ON)

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

# RESOURCES
add_compile_definitions(SHADER_PATH="${PROJECT_SOURCE_DIR}/resources/shaders/")

# VULKAN
set(VULKAN_PATH "${PROJECT_SOURCE_DIR}/external/Vulkan SDK 1.2.131.2/x86_64/")

if(${OS} STREQUAL "Windows")

  set(VULKAN_LIB ${VULKAN_LIB} "vulkan-1.lib")
  include_directories("C:/VulkanSDK/1.2.135.0/Include")
  link_directories("C:/VulkanSDK/1.2.135.0/Lib")

elseif(${OS} STREQUAL "Linux")

  set(VULKAN_LIN ${VULKAN_LIB} "libvulkan.so")
  include_directories(${VULKAN_PATH}/include)
  link_directories(${VULKAN_PATH}/lib)

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
include_directories(${PROJECT_SOURCE_DIR}/include)

# SOURCE PROJECT FILES
file(GLOB FILES_LIB
  "${PROJECT_SOURCE_DIR}/src/api/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/window/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

# LIBRARY
add_library(renderer SHARED ${FILES_LIB})

target_link_libraries(renderer
  ${WINDOWING}
  ${VULKAN_LIB}
)

set_target_properties(renderer PROPERTIES
  CXX_STANDARD 17
)

set_target_properties(renderer
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/build/lib/renderer/debug"
  LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/build/lib/renderer/debug"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/build/bin/renderer/debug"

  ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/build/lib/renderer/release"
  LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/build/lib/renderer/release"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/build/bin/renderer/release"

  ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${PROJECT_SOURCE_DIR}/build/lib/renderer/minsizerel"
  LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${PROJECT_SOURCE_DIR}/build/lib/renderer/minsizerel"
  RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${PROJECT_SOURCE_DIR}/build/bin/renderer/minsizerel"   

  ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/build/lib/renderer/relwithbebinfo"
  LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/build/lib/renderer/relwithbebinfo"
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/build/bin/renderer/relwithbebinfo"
  )

link_directories(${PROJECT_SOURCE_DIR}/build/bin/renderer/debug)

# EXAMPLE
if (BUILD_EXAMPLE)

  add_executable(example ${PROJECT_SOURCE_DIR}/src/examples/Example.cpp)

  target_link_libraries(example
    renderer
  )

  set_target_properties(example
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/build/lib/example/debug"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/build/lib/example/debug"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/build/bin/example/debug"

    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/build/lib/example/release"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/build/lib/example/release"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/build/bin/example/release"

    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${PROJECT_SOURCE_DIR}/build/lib/example/minsizerel"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${PROJECT_SOURCE_DIR}/build/lib/example/minsizerel"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${PROJECT_SOURCE_DIR}/build/bin/example/minsizerel"   

    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/build/lib/example/relwithbebinfo"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/build/lib/example/relwithbebinfo"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/build/bin/example/relwithbebinfo"
  )

endif()