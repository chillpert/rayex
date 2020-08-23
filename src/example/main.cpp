#include "Renderer.hpp"

using namespace rx;

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

class CustomCamera : public CameraBase
{
public:
  CustomCamera( const glm::ivec2& screenSize, const glm::vec3& position ) :
    CameraBase( screenSize, position ),
    m_worldUp{ 0.0f, 1.0f, 0.0f },
    m_up{ },
    m_right{ },
    m_front{ },
    m_yaw( -90.0f ),
    m_pitch( 0.0f ),
    m_sensitivity( 0.06f ),
    m_fov( 45.0f )
  {
    updateVectors( );
  }

  void update( ) override
  {
    processKeyboard( );
  }

  glm::mat4 getViewMatrix( ) const override { return glm::lookAt( m_position, m_position + m_front, m_worldUp ); }

  glm::mat4 getProjectionMatrix( ) const override
  {
    auto res = glm::perspective( glm::radians( m_fov ), static_cast< float >( m_screenSize.x ) / static_cast< float >( m_screenSize.y ), 0.1f, 100.0f );
    res[1, 1] *= -1;
    return res;
  }

  void processKeyboard( )
  {
    static float speed = 1.0f;

    float finalSpeed = speed * Time::getDeltaTime( );
    // TODO: Use switch case statement instead.
    if ( Key::eW )
      m_position += m_front * finalSpeed;

    if ( Key::eS )
      m_position -= m_front * finalSpeed;

    if ( Key::eA )
      m_position -= m_right * finalSpeed;

    if ( Key::eD )
      m_position += m_right * finalSpeed;

    if ( Key::eC )
      m_position.y -= finalSpeed / 2.0f;

    if ( Key::eSpace )
      m_position.y += finalSpeed / 2.0f;
  }

  void processMouse( float xoffset, float yoffset )
  {
    xoffset *= m_sensitivity;
    yoffset *= m_sensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if ( m_yaw > 360.0f )
      m_yaw = fmod( m_yaw, 360.0f );

    if ( m_yaw < 0.0f )
      m_yaw = 360.0f + fmod( m_yaw, 360.0f );

    if ( m_pitch > 89.0f )
      m_pitch = 89.0f;

    if ( m_pitch < -89.0f )
      m_pitch = -89.0f;

    updateVectors( );
  }

private:
  void updateVectors( )
  {
    glm::vec3 t_front;
    t_front.x = cos( glm::radians( m_yaw ) ) * cos( glm::radians( m_pitch ) );
    t_front.y = sin( glm::radians( m_pitch ) );
    t_front.z = sin( glm::radians( m_yaw ) ) * cos( glm::radians( m_pitch ) );
    m_front = glm::normalize( t_front );
    m_right = glm::normalize( glm::cross( m_front, m_worldUp ) );
    m_up = glm::normalize( glm::cross( m_right, m_front ) );
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
  CustomWindow( WindowProperties windowProps ) :
    WindowBase( windowProps ) { }

  std::shared_ptr<CustomCamera> m_camera;
  bool m_mouseVisible = true;

  void init( ) override
  {
    WindowBase::init( );

    SDL_SetRelativeMouseMode( SDL_FALSE );
  }

  bool update( ) override
  {
    // Updates the timer bound to the window as well as the window size.
    WindowBase::update( );

    m_camera->setScreenSize( glm::ivec2{ m_properties.getWidth( ), m_properties.getHeight( ) } );

    // Add your custom event polling and integrate your event system.
    SDL_Event event;

    while ( SDL_PollEvent( &event ) )
    {
      switch ( event.type )
      {
      case SDL_QUIT:
      {
        return false;
      }

      case SDL_WINDOWEVENT:
      {
        switch ( event.window.event )
        {
        case SDL_WINDOWEVENT_CLOSE:
          return false;

        case SDL_WINDOWEVENT_RESIZED:
          resize( static_cast< int >( event.window.data1 ), static_cast< int >( event.window.data2 ) );
          break;

        case SDL_WINDOWEVENT_MINIMIZED:
          resize( 0, 0 );
          break;
        }
        break;
      }

      case SDL_KEYDOWN:
      {
        switch ( event.key.keysym.sym )
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
          if ( m_mouseVisible )
          {
            m_mouseVisible = false;
            SDL_SetRelativeMouseMode( SDL_TRUE );
            SDL_GetRelativeMouseState( nullptr, nullptr ); // Magic fix!
          }
          else
          {
            SDL_SetRelativeMouseMode( SDL_FALSE );
            m_mouseVisible = true;
          }

          break;
        }
        }
        break;
      }

      case SDL_KEYUP:
      {
        switch ( event.key.keysym.sym )
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
        if ( !m_mouseVisible )
        {
          int x, y;
          SDL_GetRelativeMouseState( &x, &y );
          m_camera->processMouse( x, -y );
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
  Renderer* m_renderer;

  void configure( ) override
  {
    GuiBase::configure( );
    ImGui::StyleColorsDark( );
  }

  void render( ) override
  {
    if ( ImGui::Begin( "Settings" ) )
    {
      ImGui::SliderFloat( "Speed", &speed, 0.0f, 2.0f );

      if ( ImGui::Button( "Add Box" ) )
      {
        int max = 4;
        int min = -4;
        srand( time( NULL ) );
        int finalNum = rand( ) % ( max - min + 1 ) + min;

        auto box = std::make_shared<GeometryNode>( "models/cube.obj", Material( "textures/container.png" ) );
        box->m_worldTransform = glm::translate( box->m_worldTransform, glm::vec3( finalNum, 0.0f, 0.0f ) );
        m_renderer->pushNode( box );
      }

      if ( ImGui::Button( "Add Sphere" ) )
      {
        auto sphere = std::make_shared<GeometryNode>( "models/sphere.obj", Material( "textures/metal.png" ) );
        m_renderer->pushNode( sphere );
      }
    }

    ImGui::End( );
  }
};

int main( int argc, char* argv[] )
{
  // Window dimensions.
  int width = 900;
  int height = 600;

  // Define the window's properties according to your preferences.
  WindowProperties props( width, height, "Example", WINDOW_RESIZABLE | WINDOW_VISIBLE );
  // Now create the actual window using the window properties from above.
  auto myWindow = std::make_shared<CustomWindow>( props );
  // Create instance of your custom camera class.
  auto myCam = std::make_shared<CustomCamera>( glm::ivec2{ width, height }, glm::vec3{ 0.0f, 0.0f, 3.0f } );
  // Create the renderer object ...
  Renderer renderer( myWindow, myCam );
  // ... and initialize it.
  renderer.init( );

  // Setup your own ImGui based Gui.
  auto myGui = std::make_shared<CustomGui>();
  renderer.setGui( myGui );

  myWindow->m_camera = myCam;
  myGui->m_renderer = &renderer;

  // Setup the scene.
  auto dragonLore = std::make_shared<GeometryNode>( "models/awpdlore/awpdlore.obj", Material( "textures/awpdlore.png" ) );
  dragonLore->m_worldTransform = glm::scale( dragonLore->m_worldTransform, glm::vec3( 0.25f ) );
  dragonLore->m_worldTransform = glm::rotate( dragonLore->m_worldTransform, glm::radians( 90.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
  
  /*
  auto mars = std::make_shared<GeometryNode>( "models/sphere.obj", Material( "textures/mars.jpg" ) );
  mars->m_worldTransform = glm::scale( mars->m_worldTransform, glm::vec3( 0.25f ) );
  mars->m_worldTransform = glm::rotate( mars->m_worldTransform, glm::radians( 90.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
  mars->m_worldTransform = glm::translate( mars->m_worldTransform, glm::vec3( 0.0f, -2.0f, 0.0f ) );

  auto dirLight = std::make_shared<DirectionalLightNode>( );
  dirLight->m_ambientStrength = 0.5f;
  dirLight->m_direction = { 0.0f, 10.0f, 10.0f };
  */

  // Add the model to the renderer. This way they will be queued for rendering.
  renderer.setNodes( { dragonLore } );//, mars, dirLight } );

  while ( renderer.isRunning( ) )
  { 
    dragonLore->m_worldTransform = glm::rotate( dragonLore->m_worldTransform, glm::radians( 90.0f ) * Time::getDeltaTime( ) * speed, glm::vec3( 0.0f, 1.0f, 0.0f ) );
    //mars->m_worldTransform = glm::rotate( mars->m_worldTransform, glm::radians( 90.0f ) * Time::getDeltaTime( ) * -speed, glm::vec3( 0.0f, 1.0f, 0.0f ) );

    // Call udpate and render for the renderer to work properly.
    // This will also call your custom classes.
    renderer.update( );
    renderer.render( );
  }

  return 0;
}