#include "Renderer.hpp"

using namespace RX;

class CustomWindow : public Window
{
public:
  bool update() override
  {
    // Do your own event handling here.
    return true;
  }
};

int main(int argc, char* argv[])
{
  std::shared_ptr<Window> myWindow = std::make_shared<CustomWindow>();
  Renderer renderer(myWindow);

  renderer.initialize();

  while (renderer.isRunning())
  {
    renderer.update();
    renderer.render();
  }

  return 0;
}