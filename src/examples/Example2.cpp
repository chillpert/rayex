#include "Renderer.hpp"

using namespace RX;

int main(int argc, char* argv[])
{
  uint32_t flags = WINDOW_RESIZABLE | WINDOW_VISIBLE;
  WindowProperties windowProperties(900, 600, "Example", flags);

  std::shared_ptr<Window> myWindow = std::make_shared<Window>(windowProperties);
  Renderer renderer(myWindow);

  renderer.initialize();

  while (renderer.isRunning())
  {
    renderer.update();
    renderer.render();
  }

  return 0;
}