#include "Renderer.hpp"
#include "Camera.hpp"
#include "Model.hpp"

using namespace RX;

int width = 900;
int height = 600;
Camera cam(width, height);

// Create your own custom window class, to propagate events to your own event system.
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

    cam.setScreenDimensions(m_properties.getWidth(), m_properties.getHeight());

    // Add your custom event polling and integrate your event system.
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
      // Propagates the event to ImGui.
      //processGuiEvent(event);

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
                SDL_GetRelativeMouseState(nullptr, nullptr); // Magic fix!
              }
              else
              {
                SDL_SetRelativeMouseMode(SDL_FALSE);
                m_mouseVisible = true;
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
            cam.processMouse(x, -y);
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

  auto mars = std::make_shared<Model>();
  mars->m_pathToTexture = RX_TEXTURE_PATH "mars.jpg";
  mars->m_pathToModel = RX_MODEL_PATH "sphere.obj";

  auto rock = std::make_shared<Model>();
  rock->m_pathToTexture = RX_TEXTURE_PATH "metal.png";
  rock->m_pathToModel = RX_MODEL_PATH "rock.obj";

  rock->m_model = glm::scale(rock->m_model, glm::vec3(0.2f, 0.2f, 0.2f));
  rock->m_model = glm::translate(rock->m_model, glm::vec3(4.0f, 5.0f, 0.0f));

  // Add the model to the renderer. This way it will be queued for rendering.
  renderer.pushModel(mars);
  renderer.pushModel(rock);

  // This will set up the entire Vulkan pipeline.
  renderer.initialize();

  while (renderer.isRunning())
  {
    // Update the camera so that key inputs will have an effect on it.
    cam.update();

    // Rotate the model using the provided timer functions.
    static float speed = 0.01f;
    mars->m_model = glm::rotate(mars->m_model, glm::radians(90.0f) * Time::getDeltaTime() * speed, glm::vec3(0.0f, 1.0f, 0.0f));
    mars->m_view = cam.getViewMatrix();
    mars->m_projection = cam.getProjectionMatrix();

    rock->m_view = cam.getViewMatrix();
    rock->m_projection = cam.getProjectionMatrix();

    // Call udpate and render for the renderer to work properly.
    renderer.update();
    renderer.render();
  }

  return 0;
}