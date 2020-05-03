#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "glslang" for configuration "Release"
set_property(TARGET glslang APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(glslang PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "/root/sdk-build/1.2.131.2/x86_64/lib/libglslang.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS glslang )
list(APPEND _IMPORT_CHECK_FILES_FOR_glslang "/root/sdk-build/1.2.131.2/x86_64/lib/libglslang.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
