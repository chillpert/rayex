#include "Renderer.hpp"

using namespace rx;

float animationSpeed = 0.0f;

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
  CustomCamera( int width, int height, const glm::vec3& position ) :
    CameraBase( width, height, position ) { }

  void update( ) override
  {
    processKeyboard( );
  }

  void processKeyboard( )
  {
    static float speed = 1.0f;
    float finalSpeed = speed * Time::getDeltaTime( );

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

    updateViewMatrix( );
  }
};

class CustomWindow : public WindowBase
{
public:
  CustomWindow( int width, int height, const char* title, uint32_t flags ) :
    WindowBase( width, height, title, flags )
  { }

  void init( ) override
  {
    WindowBase::init( );

    SDL_SetRelativeMouseMode( SDL_FALSE );
  }

  bool update( ) override
  {
    WindowBase::update( );

    m_camera->setSize( m_width, m_height );

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
              resize( static_cast<int>( event.window.data1 ), static_cast<int>( event.window.data2 ) );
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

  void setCamera( const std::shared_ptr<CameraBase> camera )
  {
    m_camera = camera;
  }

private:
  std::shared_ptr<CameraBase> m_camera;
  bool m_mouseVisible = true;
};

class CustomGui : public GuiBase
{
private:
  void configure( ) override
  {
    GuiBase::configure( );
    ImGui::StyleColorsDark( );
  }

  void render( ) override
  {
    if ( ImGui::Begin( "Settings" ) )
    {
      ImGui::SliderFloat( "Speed", &animationSpeed, 0.0f, 2.0f );

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

public:
  void setRenderer( Renderer* renderer )
  {
    m_renderer = renderer;
  }

private:
  Renderer* m_renderer;
};

int main( int argc, char* argv[] )
{
  // Window dimensions.
  int width = 900;
  int height = 600;

  // Now create the actual window using the window properties from above.
  auto myWindow = std::make_shared<CustomWindow>( width, height, "Example", WINDOW_RESIZABLE | WINDOW_INPUT_FOCUS );

  // Create instance of your custom camera class.
  auto myCam = std::make_shared<CustomCamera>( width, height, glm::vec3( 0.0f, 0.0f, 3.0f ) );

  // Create the renderer object ...
  Renderer myRenderer( myWindow, myCam );

  // ... and initialize it.
  myRenderer.init( );

  // Setup your own ImGui based Gui.
  auto myGui = std::make_shared<CustomGui>( );
  myRenderer.setGui( myGui );

  myGui->setRenderer( &myRenderer );
  myWindow->setCamera( myCam );

  // Setup the scene
  auto dragonLore = std::make_shared<GeometryNode>( "models/awpdlore/awpdlore.obj", Material( "textures/awpdlore.png" ) );
  dragonLore->m_worldTransform = glm::scale( dragonLore->m_worldTransform, glm::vec3( 0.25f ) );
  dragonLore->m_worldTransform = glm::rotate( dragonLore->m_worldTransform, glm::radians( 90.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );

  // Add the model to the renderer. This way they will be queued for rendering.
  myRenderer.pushNode( dragonLore );

  while ( myRenderer.isRunning( ) )
  {
    dragonLore->m_worldTransform = glm::rotate( dragonLore->m_worldTransform, glm::radians( 90.0f ) * Time::getDeltaTime( ) * animationSpeed, glm::vec3( 0.0f, 1.0f, 0.0f ) );
    myRenderer.run( );
  }

  return 0;
}