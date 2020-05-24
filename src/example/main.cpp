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

  bool m_mouseVisible = true;

  void initialize() override
  {
    Window::initialize();

    SDL_SetRelativeMouseMode(SDL_FALSE);
  }

  bool update() override
  {
    // Updates the timer bound to the window as well as the window size.
    Window::update();
        
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

            case SDLK_ESCAPE:
              return false;

            case SDLK_SPACE:
            {
              if (m_mouseVisible)
              {
                m_mouseVisible = false;
                SDL_SetRelativeMouseMode(SDL_TRUE);
              }
              else
              {
                m_mouseVisible = true;
                SDL_SetRelativeMouseMode(SDL_FALSE);
              }

              break;
            }
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
          if (!m_mouseVisible)
          {
            int x, y;
            SDL_GetRelativeMouseState(&x, &y);
            cam.processMouse(x, y);
            break;
          }
        }
      }
    }
    return true;
  }
};

int main(int argc, char* argv[])
{
  // Define the window's properties according to your preferences.
  WindowProperties props(width, height, "Example", WINDOW_RESIZABLE | WINDOW_VISIBLE);
  // Now create the actual window using the window properties from above.
  auto myWindow = std::make_shared<CustomWindow>(props);
  // Create the renderer object.
  Renderer renderer(myWindow);

  // Create a rectangle.
  auto rectangle = std::make_shared<Model>();
  rectangle->vertices = 
  {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}}
  };
  rectangle->indices = { 0, 1, 2, 2, 3, 0 };
  
  // Add the rectangle to the renderer. This way it will be queued for rendering.
  renderer.pushModel(rectangle);

  // This will set up the entire Vulkan pipeline.
  renderer.initialize();

  while (renderer.isRunning())
  {
    // Update the camera so that key inputs will have an effect on it.
    cam.update();

    // Rotate the rectangle using the provided timer functions.
    static float startTime = Time::getTime();
    float currentTime = Time::getTime();
    float time = currentTime - startTime;

    rectangle->ubo->model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    rectangle->ubo->view = cam.getViewMatrix();
    rectangle->ubo->projection = cam.getProjectionMatrix();

    // Call udpate and render for the renderer to work properly.
    renderer.update();
    renderer.render();
  }

  return 0;
}