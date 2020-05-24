#include "Renderer.hpp"
#include "Camera.hpp"
#include "Model.hpp"

using namespace RX;

int width = 900;
int height = 600;
Camera cam(static_cast<float>(width), static_cast<float>(height));

// Create your own custom window class, to propagate it to your own event system.  
class CustomWindow : public Window
{
public:
  CustomWindow(WindowProperties windowProps) : 
    Window(windowProps) { }

  void initialize() override
  {
    Window::initialize();

    
  }

  bool update() override
  {
    // Updates the timer bound to the window as well as the window size.
    Window::update();
    
    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    // Add your custom event polling and integrate your event system.
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
        {
          return false;
        }

        case SDL_WINDOWEVENT:
        {
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

        case SDL_KEYDOWN:
        {
          switch (event.key.keysym.sym)
          {
            case SDLK_w:
              KEY::w = true;
              break;

            case SDLK_a:
              KEY::a = true;
              break;

            case SDLK_s:
              KEY::s = true;
              break;

            case SDLK_d:
              KEY::d = true;
              break;
          }
          break;
        }

        case SDL_KEYUP:
        {
          switch (event.key.keysym.sym)
          {
            case SDLK_w:
              KEY::w = false;
              break;

            case SDLK_a:
              KEY::a = false;
              break;

            case SDLK_s:
              KEY::s = false;
              break;

            case SDLK_d:
              KEY::d = false;
              break;
          }
          break;
        }

        case SDL_MOUSEMOTION:
        {
          int x, y;
          SDL_GetRelativeMouseState(&x, &y);
          cam.processMouse(x, -y);
          break;
        }
      }
    }
    return true;
  }
};

int main(int argc, char* argv[])
{
  // Create the window.
  WindowProperties props(width, height, "Example", WINDOW_RESIZABLE | WINDOW_VISIBLE);
  std::shared_ptr<Window> myWindow = std::make_shared<CustomWindow>(props);

  std::vector<Vertex> vertices =
  {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
  };

  std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };

  Renderer renderer(myWindow);

  std::shared_ptr<Model> rectangle = std::make_shared<Model>();
  rectangle->vertices = vertices;
  rectangle->indices = indices;
  renderer.pushModel(rectangle);

  renderer.initialize();

  while (renderer.isRunning())
  {
    cam.update();

    static float startTime = Time::getTime();

    float currentTime = Time::getTime();
    float time = currentTime - startTime;

    rectangle->ubo->model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    rectangle->ubo->view = cam.getViewMatrix();
    rectangle->ubo->projection = cam.getProjectionMatrix();
    rectangle->ubo->projection[1][1] *= -1;

    renderer.update();
    renderer.render();
  }

  return 0;
}