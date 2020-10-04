cmake_minimum_required(VERSION 3.16.2)

# This makes it possible to use relative paths which usually start from the application's location. This may vary depending on your setup.
option(COPY_ASSETS_TO_BINARY "Copy assets to binary" ON)

if(COPY_ASSETS_TO_BINARY)
  add_compile_definitions(RX_COPY_ASSETS)
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

include_directories(${PROJECT_SOURCE_DIR}/example)

file(GLOB FILES_EXAMPLE
  "${PROJECT_SOURCE_DIR}/example/*.cpp"
)

add_executable(example ${PROJECT_SOURCE_DIR}/example/main.cpp ${FILES_EXAMPLE})

if(${OS} STREQUAL "Windows")
  target_link_libraries(example
      "${PROJECT_SOURCE_DIR}/build/lib/${BUILD_PATH}/renderer.lib"
      ${WINDOWING}
  )
elseif(${OS} STREQUAL "Linux")
  target_link_libraries(example
      renderer
      ${WINDOWING}
  )
endif()

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