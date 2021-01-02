cmake_minimum_required(VERSION 3.16.2)

# SOURCE PROJECT FILES
file(GLOB FILES_LIB
  "${PROJECT_SOURCE_DIR}/source/api/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/api/pathtrace/*.cpp"

  "${PROJECT_SOURCE_DIR}/source/pch/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/events/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/utils/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/*.cpp"
  "${PROJECT_SOURCE_DIR}/source/base/*.cpp"

  # ImGui files
  "${PROJECT_SOURCE_DIR}/source/external/imgui/*.cpp"
)