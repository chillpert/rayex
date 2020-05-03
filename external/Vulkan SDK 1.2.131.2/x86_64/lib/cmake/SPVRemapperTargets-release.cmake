#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SPVRemapper" for configuration "Release"
set_property(TARGET SPVRemapper APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SPVRemapper PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "/root/sdk-build/1.2.131.2/x86_64/lib/libSPVRemapper.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS SPVRemapper )
list(APPEND _IMPORT_CHECK_FILES_FOR_SPVRemapper "/root/sdk-build/1.2.131.2/x86_64/lib/libSPVRemapper.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
