#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "OGLCompiler" for configuration "Release"
set_property(TARGET OGLCompiler APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(OGLCompiler PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "/root/sdk-build/1.2.131.2/x86_64/lib/libOGLCompiler.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS OGLCompiler )
list(APPEND _IMPORT_CHECK_FILES_FOR_OGLCompiler "/root/sdk-build/1.2.131.2/x86_64/lib/libOGLCompiler.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
