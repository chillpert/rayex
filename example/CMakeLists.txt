cmake_minimum_required(VERSION 3.16.2)

file(GLOB FILES_EXAMPLE
  "${PROJECT_SOURCE_DIR}/example/*.cpp"
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

# Copy DLLs to binary location in post build for Windows
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
add_custom_command(
  TARGET example
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
          ${PROJECT_SOURCE_DIR}/include/external/libs/SDL2.dll
          ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/SDL2.dll
  COMMENT "Running POST_BUILD action: Copying SDL2d.dll to binary location"
  )

add_custom_command(
  TARGET example
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
          ${PROJECT_SOURCE_DIR}/include/external/libs/ktx.dll
          ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/ktx.dll
  COMMENT "Running POST_BUILD action: Copying SDL2d.dll to binary location"
  )
endif()