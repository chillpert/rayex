#include "Renderer.hpp"
#include "Camera.hpp"

using namespace RX;

class CustomWindow : public Window
{
public:
  CustomWindow(WindowProperties windowProps) : 
    Window(windowProps) { }

  bool update() override
  {
    Window::update();

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT:
        return false;

      case SDL_WINDOWEVENT:
        switch (event.window.event)
        {
        case SDL_WINDOWEVENT_CLOSE:
          return false;

        case SDL_WINDOWEVENT_RESIZED:
          resize(static_cast<int>(event.window.data1), static_cast<int>(event.window.data2));
          break;

        case SDL_WINDOWEVENT_MINIMIZED:
          resize(0, 0);
          break;
        }
        break;
      }
    }
    return true;
  }
};

int main(int argc, char* argv[])
{  
  WindowProperties props(900, 600, "Example", WINDOW_RESIZABLE | WINDOW_VISIBLE);

  std::shared_ptr<Window> myWindow = std::make_shared<CustomWindow>(props);

  Renderer renderer(myWindow);

  renderer.initialize();

  while (renderer.isRunning())
  {
    renderer.update();
    renderer.render();
  }

  return 0;
}