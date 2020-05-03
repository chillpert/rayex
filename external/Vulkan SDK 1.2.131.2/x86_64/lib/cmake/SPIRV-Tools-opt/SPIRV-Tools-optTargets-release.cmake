#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SPIRV-Tools-opt" for configuration "Release"
set_property(TARGET SPIRV-Tools-opt APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SPIRV-Tools-opt PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "/root/sdk-build/1.2.131.2/x86_64/lib/libSPIRV-Tools-opt.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS SPIRV-Tools-opt )
list(APPEND _IMPORT_CHECK_FILES_FOR_SPIRV-Tools-opt "/root/sdk-build/1.2.131.2/x86_64/lib/libSPIRV-Tools-opt.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
