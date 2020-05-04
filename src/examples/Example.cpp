#include "Renderer.hpp"

using namespace RX;

int main(int argc, char* argv[])
{
  uint32_t flags = WINDOW_RESIZABLE | WINDOW_VISIBLE;
  WindowProperties windowProperties(300, 200, "Example", flags);

  Renderer renderer(Api::VULKAN, windowProperties);

  try
  {
    renderer.initialize();

    while (true)
    {

    }
  }
  catch (std::exception e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}