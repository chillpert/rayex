cmake_minimum_required(VERSION 3.16.2)

file(GLOB FILES_EXAMPLE
  "${PROJECT_SOURCE_DIR}/example/*.cpp"

  # ImGui files (again because MSVC for whatever reason requires them here as well)
  "${PROJECT_SOURCE_DIR}/include/external/imgui/*.cpp"
  "${PROJECT_SOURCE_DIR}/include/external/imgui/backends/imgui_impl_sdl.cpp"
  "${PROJECT_SOURCE_DIR}/include/external/imgui/backends/imgui_impl_vulkan.cpp"
)

add_executable(example ${PROJECT_SOURCE_DIR}/example/main.cpp ${FILES_EXAMPLE})

# Include directories for example
target_include_directories(example PRIVATE
  ${PROJECT_SOURCE_DIR}/example
)

target_link_libraries(example
  rayex
)

set_target_properties(example PROPERTIES
  CXX_STANDARD 20

  ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}"
  LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}"
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}"
)

# WARNING: On Windows the output folder is created twice for whatever reason. So the post-build copy command was adjusted accordingly.
# Copy DLLs to binary location in post build for Windows
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
add_custom_command(
  TARGET example
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
          ${PROJECT_SOURCE_DIR}/include/external/libs/SDL2.dll
          ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/${CMAKE_BUILD_TYPE}/SDL2.dll
  COMMENT "Running POST_BUILD action: Copying SDL2d.dll to binary location"
  )

add_custom_command(
  TARGET example
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
          ${PROJECT_SOURCE_DIR}/include/external/libs/ktx.dll
          ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/${CMAKE_BUILD_TYPE}/ktx.dll
  COMMENT "Running POST_BUILD action: Copying ktx.dll to binary location"
  )
endif()