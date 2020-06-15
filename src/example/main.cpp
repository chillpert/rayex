#include "Renderer.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

using namespace RX;

float speed = 0.0f;

namespace Key
{
  bool eW;
  bool eA;
  bool eS;
  bool eD;
  bool eC;
  bool eSpace;
}

class CustomCamera : public RX::CameraBase
{
public:
  CustomCamera(const glm::ivec2& screenSize, const glm::vec3& position) :
    CameraBase(screenSize, position),
    m_worldUp{ 0.0f, 1.0f, 0.0f },
    m_up{ },
    m_right{ },
    m_front{ },
    m_yaw(-90.0f),
    m_pitch(0.0f),
    m_sensitivity(0.06f),
    m_fov(45.0f)
  { 
    updateVectors(); 
  }

  void update() override
  {
    processKeyboard();
  }

  inline const glm::mat4& getViewMatrix() override { return glm::lookAt(m_position, m_position + m_front, m_worldUp); }

  const glm::mat4& getProjectionMatrix()
  {
    auto res = glm::perspective(glm::radians(m_fov), static_cast<float>(m_screenSize.x) / static_cast<float>(m_screenSize.y), 0.1f, 100.0f);
    res[1, 1] *= -1;
    return res;
  }

  void processKeyboard()
  {
    static float speed = 1.0f;

    float finalSpeed = speed * RX::Time::getDeltaTime();
    // TODO: Use switch case statement instead.
    if (Key::eW)
      m_position += m_front * finalSpeed;

    if (Key::eS)
      m_position -= m_front * finalSpeed;

    if (Key::eA)
      m_position -= m_right * finalSpeed;

    if (Key::eD)
      m_position += m_right * finalSpeed;

    if (Key::eC)
      m_position.y -= finalSpeed / 2.0f;

    if (Key::eSpace)
      m_position.y += finalSpeed / 2.0f;
  }

  void processMouse(float xoffset, float yoffset)
  {
    xoffset *= m_sensitivity;
    yoffset *= m_sensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (m_yaw > 360.0f)
      m_yaw = fmod(m_yaw, 360.0f);

    if (m_yaw < 0.0f)
      m_yaw = 360.0f + fmod(m_yaw, 360.0f);

    if (m_pitch > 89.0f)
      m_pitch = 89.0f;

    if (m_pitch < -89.0f)
      m_pitch = -89.0f;

    updateVectors();
  }

private:
  void updateVectors()
  {
    glm::vec3 t_front;
    t_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    t_front.y = sin(glm::radians(m_pitch));
    t_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(t_front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
  }

  glm::fvec3 m_worldUp;
  glm::fvec3 m_up;
  glm::fvec3 m_right;

public:
  glm::fvec3 m_front;

  float m_yaw;
  float m_pitch;
  float m_sensitivity;
  float m_fov;
};

// Create your own custom window class, to propagate events to your own event system.
class CustomWindow : public WindowBase
{
public:
  CustomWindow(WindowProperties windowProps) : 
    WindowBase(windowProps) { }

  std::shared_ptr<CustomCamera> m_camera;
  bool m_mouseVisible = true;

  void initialize() override
  {
    WindowBase::initialize();

    SDL_SetRelativeMouseMode(SDL_FALSE);
  }

  bool update() override
  {
    // Updates the timer bound to the window as well as the window size.
    WindowBase::update();

    m_camera->setScreenSize(glm::ivec2{ m_properties.getWidth(), m_properties.getHeight() } );
    
    // Add your custom event polling and integrate your event system.
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
      // Propagates the event to ImGui.
      processGuiEvent(event);

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
              Key::eW = true;
              break;

            case SDLK_a:
              Key::eA = true;
              break;

            case SDLK_s:
              Key::eS = true;
              break;

            case SDLK_d:
              Key::eD = true;
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
              Key::eW = false;
              break;

            case SDLK_a:
              Key::eA = false;
              break;

            case SDLK_s:
              Key::eS = false;
              break;

            case SDLK_d:
              Key::eD = false;
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
            m_camera->processMouse(x, -y);
            break;
          }
        }
      }
    }
    return true;
  }
};

class CustomGui : public GuiBase
{
public:
  void configure() override
  {
    GuiBase::configure();
    ImGui::StyleColorsDark();
  }

  void render() override
  {
    if (ImGui::Begin("Settings"))
    {
      ImGui::SliderFloat("Speed", &speed, 0.0f, 2.0f);
    }

    ImGui::End();
  }
};

// Custom model class that only parses obj files using tinyObjLoader.
class CustomModel : public ModelBase
{
public:
  CustomModel(const std::string& path) :
    ModelBase(path)
  {
    initialize();
  }

  void initialize() override
  {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, m_path.c_str()))
      RX_ERROR(warn + err);

    std::unordered_map<Vertex, uint32_t> uniqueVertices;

    for (const auto& shape : shapes)
    {
      for (const auto& index : shape.mesh.indices)
      {
        Vertex vertex{};

        vertex.pos =
        {
          attrib.vertices[3 * index.vertex_index + 0],
          attrib.vertices[3 * index.vertex_index + 1],
          attrib.vertices[3 * index.vertex_index + 2]
        };

        vertex.normal =
        {
          attrib.normals[3 * index.normal_index + 0],
          attrib.normals[3 * index.normal_index + 1],
          attrib.normals[3 * index.normal_index + 2]
        };

        vertex.texCoord =
        {
          attrib.texcoords[2 * index.texcoord_index + 0],
          1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
        };

        vertex.color = { 1.0f, 1.0f, 1.0f };

        if (uniqueVertices.count(vertex) == 0)
        {
          uniqueVertices[vertex] = static_cast<uint32_t>(m_vertices.size());
          m_vertices.push_back(vertex);
        }

        m_indices.push_back(uniqueVertices[vertex]);
      }
    }
  }
};

int main(int argc, char* argv[])
{
  // Screen dimensions.
  int width = 900;
  int height = 600;

  // Define the window's properties according to your preferences.
  WindowProperties props(width, height, "Example", WINDOW_RESIZABLE | WINDOW_VISIBLE);
  // Now create the actual window using the window properties from above.
  auto myWindow = std::make_shared<CustomWindow>(props);
  // Setup your own ImGui based Gui.
  auto myGui = std::make_unique<CustomGui>();

  auto myCam = std::make_shared<CustomCamera>(glm::ivec2{ width, height }, glm::vec3{ 0.0f, 0.0f, 3.0f });
  myWindow->m_camera = myCam;
  // Create the renderer object.
  Renderer renderer(myWindow, std::move(myGui), myCam);
  
  auto dragonLore = std::make_shared<GeometryNodeBase>();
  dragonLore->m_model = std::make_shared<CustomModel>(RX_MODEL_PATH "awpdlore/awpdlore.obj");
  dragonLore->m_material.diffuseTexture = RX_TEXTURE_PATH "awpdlore.png";

  dragonLore->m_worldTransform = glm::scale(dragonLore->m_worldTransform, glm::vec3(0.25f));
  dragonLore->m_worldTransform = glm::rotate(dragonLore->m_worldTransform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  auto mars = std::make_shared<GeometryNodeBase>();
  mars->m_model = std::make_shared<CustomModel>(RX_MODEL_PATH "sphere.obj");
  mars->m_material.diffuseTexture = RX_TEXTURE_PATH "mars.jpg";

  mars->m_worldTransform = glm::scale(mars->m_worldTransform, glm::vec3(0.25f));
  mars->m_worldTransform = glm::rotate(mars->m_worldTransform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  mars->m_worldTransform = glm::translate(mars->m_worldTransform, glm::vec3(0.0f, -2.0f, 0.0f));

  // Add the model to the renderer. This way they will be queued for rendering.
  renderer.setNodes({ dragonLore, mars });

  renderer.initialize();

  while (renderer.isRunning())
  {
    // Update the camera so that key inputs will have an effect on it.    
    dragonLore->m_worldTransform = glm::rotate(dragonLore->m_worldTransform, glm::radians(90.0f) * Time::getDeltaTime() * speed, glm::vec3(0.0f, 1.0f, 0.0f));
    mars->m_worldTransform = glm::rotate(mars->m_worldTransform, glm::radians(90.0f) * Time::getDeltaTime() * -speed, glm::vec3(0.0f, 1.0f, 0.0f));

    // Call udpate and render for the renderer to work properly.
    renderer.update();
    renderer.render();
  }

  return 0;
}