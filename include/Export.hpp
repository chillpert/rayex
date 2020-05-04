#ifndef EXPORT_HPP
#define EXPORT_HPP

#ifdef RX_PLATFORM_WINDOWS_X64
  #ifdef RX_BUILD_DLL
    #define RX_API __declspec(dllexport)
  #else
    #define RX_API __declspec(dllimport)
  #endif
#elif RX_PLATFORM_UNIX_X64
  #ifdef RX_BUILD_DLL
    #define RX_API __attribute__((visibility("default")))
  #else   
    #define RX_API
  #endif
#endif

#endif // EXPORT_HPP