cmake_minimum_required(VERSION 3.16.2)

# SOURCE PROJECT FILES
file(GLOB FILES_LIB
  "${PROJECT_SOURCE_DIR}/source/api/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/api/buffers/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/api/descriptors/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/api/devices/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/api/gui/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/api/image/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/api/misc/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/api/model/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/api/raytracing/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/api/utility/*.cpp"

  "${PROJECT_SOURCE_DIR}/source/window/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/pch/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/events/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/utils/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/base/*.cpp"

  # ImGui files
  "${PROJECT_SOURCE_DIR}/external/ImGui/*.cpp"
  "${PROJECT_SOURCE_DIR}/external/tinyLogger/*.cpp"
)