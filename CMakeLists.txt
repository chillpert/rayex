cmake_minimum_required(VERSION 3.14.0)
project(Renderer VERSION 0.0.1)

option(BUILD_EXAMPLE "Build example" ON)

# PATHS
add_compile_definitions(SHADER_PATH="${PROJECT_SOURCE_DIR}/resources/shaders/")

# VULKAN
set(VULKAN_PATH "${PROJECT_SOURCE_DIR}/external/Vulkan SDK 1.2.131.2/x86_64/")

include_directories(${VULKAN_PATH}/include)
link_directories(${VULKAN_PATH}/lib)

# SDL
if(WIN32 OR MSVC)

  link_directories(${PROJECT_SOURCE_DIR}/external/SDL2/binaries/win64)
  include_directories(${PROJECT_SOURCE_DIR}/external/SDL2/include)

  set(WINDOWING ${WINDOWING} "SDL2.dll")

elseif(UNIX)

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
  libvulkan.so
)

set_target_properties(renderer PROPERTIES
  CXX_STANDARD 17
)

# EXAMPLE
if (BUILD_EXAMPLE)

  add_executable(example ${PROJECT_SOURCE_DIR}/src/examples/Example.cpp)

  target_link_libraries(example
    renderer
  )

endif()