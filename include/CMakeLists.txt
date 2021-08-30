cmake_minimum_required(VERSION 3.16)

message("Including Rayex headers from " ${CMAKE_CURRENT_LIST_DIR})

include_directories(
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/external
  ${CMAKE_CURRENT_LIST_DIR}/external/imgui
  ${CMAKE_CURRENT_LIST_DIR}/external/imgui/backends
)