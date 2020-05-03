#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "glslangValidator" for configuration "Release"
set_property(TARGET glslangValidator APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(glslangValidator PROPERTIES
  IMPORTED_LOCATION_RELEASE "/root/sdk-build/1.2.131.2/x86_64/bin/glslangValidator"
  )

list(APPEND _IMPORT_CHECK_TARGETS glslangValidator )
list(APPEND _IMPORT_CHECK_FILES_FOR_glslangValidator "/root/sdk-build/1.2.131.2/x86_64/bin/glslangValidator" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
