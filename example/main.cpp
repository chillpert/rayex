#include "RayExec.hpp"

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

class CustomCamera : public Camera
{
public:
  CustomCamera( int width, int height, const glm::vec3& position ) :
    Camera( width, height, position ) { }

  void update( ) override
  {
    processKeyboard( );
  }

  void processKeyboard( )
  {
    static float speed = 1.0f;
    float finalSpeed = speed * Time::getDeltaTime( );

    if ( Key::eW )
    {
      this->position += this->front * finalSpeed;
      this->updateView = true;
    }

    if ( Key::eS )
    {
      this->position -= this->front * finalSpeed;
      this->updateView = true;
    }
    
    if ( Key::eA )
    {
      this->position -= this->right * finalSpeed;
      this->updateView = true;
    }
    
    if ( Key::eD )
    {
      this->position += this->right * finalSpeed;
      this->updateView = true;
    }
    
    if ( Key::eC )
    {
      this->position.y -= finalSpeed / 2.0f;
      this->updateView = true;
    }
    
    if ( Key::eSpace )
    {
      this->position.y += finalSpeed / 2.0f;
      this->updateView = true;
    }
    
    updateViewMatrix( );
  }
};

class CustomWindow : public Window
{
public:
  CustomWindow( int width, int height, const char* title, uint32_t flags ) :
    Window( width, height, title, flags )
  { }

  bool init( ) override
  {
    if ( !Window::init( ) ) return false;

    SDL_SetRelativeMouseMode( SDL_FALSE );
    return true;
  }

  bool update( ) override
  {
    if ( !Window::update( ) ) return false;

    this->camera->setSize( this->width, this->height );

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
              if ( this->mouseVisible )
              {
                this->mouseVisible = false;
                SDL_SetRelativeMouseMode( SDL_TRUE );
                SDL_GetRelativeMouseState( nullptr, nullptr ); // Magic fix!
              }
              else
              {
                SDL_SetRelativeMouseMode( SDL_FALSE );
                this->mouseVisible = true;
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
          if ( !this->mouseVisible )
          {
            int x, y;
            SDL_GetRelativeMouseState( &x, &y );
            this->camera->processMouse( x, -y );
            break;
          }
        }
      }
    }
    return true;
  }

  void setCamera( const std::shared_ptr<Camera> camera )
  {
    this->camera = camera;
  }

private:
  std::shared_ptr<Camera> camera;
  bool mouseVisible = true;
};

class CustomGui : public Gui
{
private:
  void configure( ) override
  {
    Gui::configure( );
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
        box->worldTransform = glm::translate( box->worldTransform, glm::vec3( finalNum, 0.0f, 0.0f ) );
        this->renderer->pushNode( box );
      }

      if ( ImGui::Button( "Add Sphere" ) )
      {
        auto sphere = std::make_shared<GeometryNode>( "models/sphere.obj", Material( "textures/metal.png" ) );
        this->renderer->pushNode( sphere );
      }

      auto clearColor = this->renderer->settings.getClearColor( );
      if ( ImGui::ColorEdit4( "##AmbientColor", &clearColor[0] ) )
      {
        this->renderer->settings.setClearColor( clearColor );
      }

      if ( ImGui::Checkbox( "RayTracing", &this->renderer->settings.rayTrace ) )
      {
        this->renderer->settings.refresh = true;
      }
    }

    ImGui::End( );
  }

public:
  void setRenderer( RayExec* renderer )
  {
    this->renderer = renderer;
  }

private:
  RayExec* renderer;
};

int main( )
{
  // Window dimensions.
  int width = 900;
  int height = 600;
  
  // Now create the actual window using the window properties from above.
  auto myWindow = std::make_shared<CustomWindow>( width, height, "Example", SDL_WINDOW_RESIZABLE );

  // Create instance of your custom camera class.
  auto myCam = std::make_shared<CustomCamera>( width, height, glm::vec3( 0.0f, 0.0f, 3.0f ) );

  // Create the renderer object ...
  RayExec renderer( myWindow, myCam );

  // ... and initialize it.
  renderer.init( );

  // Setup your own ImGui based Gui.
  auto myGui = std::make_shared<CustomGui>( );
  renderer.setGui( myGui );

  myGui->setRenderer( &renderer);
  myWindow->setCamera( myCam );
  
  // Setup the scene
  auto dragonLore = std::make_shared<GeometryNode>( "models/awpdlore/awpdlore.obj", Material( "textures/awpdlore.png" ) );
  dragonLore->worldTransform = glm::scale( dragonLore->worldTransform, glm::vec3( 0.25f ) );
  dragonLore->worldTransform = glm::rotate( dragonLore->worldTransform, glm::radians( 90.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
  
  auto directionalLight = std::make_shared<DirectionalLightNode>( );
 
  // Add the model to the renderer. This way they will be queued for rendering.
  renderer.pushNode( dragonLore );
  renderer.pushNode( directionalLight );

  while ( renderer.isRunning( ) )
  {
    //dragonLore->worldTransform = glm::rotate( dragonLore->worldTransform, glm::radians( 90.0f ) * Time::getDeltaTime( ) * animationSpeed, glm::vec3( 0.0f, 1.0f, 0.0f ) );
    renderer.run( );
  }

  return 0;
}