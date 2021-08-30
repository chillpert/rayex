cmake_minimum_required(VERSION 3.16)

file(GLOB IMGUI_SOURCES
  "${CMAKE_CURRENT_LIST_DIR}/external/imgui/*.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/external/imgui/backends/imgui_impl_sdl.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/external/imgui/backends/imgui_impl_vulkan.cpp"
)

# Vulkan
find_package(Vulkan REQUIRED)

# SDL
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  find_package(SDL2 REQUIRED)
else()
  set(SDL2_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/external/SDL/include)
  set(SDL2_LIBRARIES "SDL2.lib")
endif()

# KTX
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(KTX "libktx.so" "libktx_read.so")  
else()
  set(KTX "ktx.lib")
endif()

message("Including Rayex headers from " ${CMAKE_CURRENT_LIST_DIR})

include_directories(
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/external
  ${CMAKE_CURRENT_LIST_DIR}/external/imgui
  ${CMAKE_CURRENT_LIST_DIR}/external/imgui/backends
)

set(RAYEX_INCLUDE_DIRS
  ${Vulkan_INCLUDE_DIRS}
  ${SDL2_INCLUDE_DIRS}
)

set(RAYEX_EXTERNAL_LINK_DIRS
  ${CMAKE_CURRENT_LIST_DIR}/external/libs
)

set(RAYEX_LIBRARY
  $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Linux>:librayex.so>
  $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>:rayex.lib>
)

set(RAYEX_EXTERNAL_LIBRARIES 
  ${Vulkan_LIBRARY}
  ${SDL2_LIBRARIES}
  ${KTX}
)