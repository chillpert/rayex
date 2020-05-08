#include "Renderer.hpp"

using namespace RX;

int main(int argc, char* argv[])
{
  uint32_t flags = WINDOW_RESIZABLE | WINDOW_VISIBLE;
  WindowProperties windowProperties(900, 600, "Example", flags);

  Renderer renderer(windowProperties);

  try
  {
    while (renderer.isRunning())
    {
      renderer.update();
      renderer.render();
    }
  }
  catch (std::exception e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}