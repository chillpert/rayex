#include "Renderer.hpp"

using namespace RX;

int main(int argc, char* argv[])
{
  Renderer renderer;

  renderer.initialize();

  while (renderer.isRunning())
  {
    renderer.update();
    renderer.render();
  }

  return 0;
}