cmake_minimum_required(VERSION 3.16)

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

set(RAYEX_INCLUDE_DIRS
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/external
  ${CMAKE_CURRENT_LIST_DIR}/external/imgui
  #${CMAKE_CURRENT_LIST_DIR}/external/imgui/backends

  ${Vulkan_INCLUDE_DIRS}
  ${SDL2_INCLUDE_DIRS}
)

set(RAYEX_LINK_DIRS
  $<$<STREQUAL:${BUILDING_RAYEX},>:${PATH_TO_RAYEX}/build>

  ${CMAKE_CURRENT_LIST_DIR}/external/libs
)

set(RAYEX_LIBRARIES
  $<$<AND:$<STREQUAL:${BUILDING_RAYEX},>,$<STREQUAL:${CMAKE_SYSTEM_NAME},Linux>>:librayex.so>
  $<$<AND:$<STREQUAL:${BUILDING_RAYEX},>,$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>>:rayex.lib>
  
  ${Vulkan_LIBRARY}
  ${SDL2_LIBRARIES}
  ${KTX}
)
