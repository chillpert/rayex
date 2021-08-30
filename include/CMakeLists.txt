cmake_minimum_required(VERSION 3.16)

# Vulkan
find_package(Vulkan REQUIRED)

# SDL
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  find_package(SDL2 REQUIRED)
else()
  set(SDL2_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/external/SDL/include)
  set(SDL2_LIBRARIES "SDL2main.lib" "SDL2.lib")
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

set(RAYEX_LINK_DIRECTORIES
  ${CMAKE_CURRENT_LIST_DIR}/../build
  ${CMAKE_CURRENT_LIST_DIR}/external/libs
)

set(RAYEX_LIBRARIES 
  $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Linux>:librayex.so>
  $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>:rayex.lib>

  ${Vulkan_LIBRARY}
  ${SDL2_LIBRARIES}
  ${KTX}
)