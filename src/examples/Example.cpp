#include "Renderer.hpp"

using namespace RX;

int main(int argc, char* argv[])
{
  Renderer renderer(API::VULKAN, 300, 200, "Example");

  try
  {
    renderer.initialize();
  }
  catch (std::exception e)
  {
    std::cerr << e.what() << std::endl;
  }


  while (true)
  {
    
  }

  return 0;
}