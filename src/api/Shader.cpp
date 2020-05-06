#include "api/Shader.hpp"

namespace RX
{
  Shader::Shader(const char* path) :
    m_path(path) 
  {

  }

  void Shader::parse()
  {
    /*
#ifdef RX_PLATFORM_WINDOWS_X64
#define FINAL VALIDATOR_PATH ## .exe
    std::string command = FINAL;
    std::system(command.c_str());
#elif RX_PLATFORM_LINUX_X64
    
#endif
    */
  }
}