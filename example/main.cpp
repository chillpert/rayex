#include "RayExec.hpp"

using namespace rx;

float animationSpeed = 0.0F;

namespace Key
{
  bool eW;
  bool eA;
  bool eS;
  bool eD;
  bool eC;
  bool eSpace;
  bool eLeftShift;
} // namespace Key

class CustomCamera : public Camera
{
public:
  CustomCamera( int width, int height, const glm::vec3& position ) :
    Camera( width, height, position ) {}

  void update( ) override
  {
    Camera::update( );

    processKeyboard( );
  }

  void processKeyboard( )
  {
    const float defaultSpeed  = 2.5F;
    static float currentSpeed = defaultSpeed;
    float finalSpeed          = currentSpeed * Time::getDeltaTime( );

    if ( Key::eLeftShift )
    {
      currentSpeed = 10.0F;
    }
    else
    {
      currentSpeed = defaultSpeed;
    }

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
      this->position.y -= finalSpeed / 2.0F;
      this->updateView = true;
    }

    if ( Key::eSpace )
    {
      this->position.y += finalSpeed / 2.0F;
      this->updateView = true;
    }

    updateViewMatrix( );
  }
};

class CustomWindow : public Window
{
public:
  CustomWindow( int width, int height, const char* title, uint32_t flags, std::shared_ptr<Camera> camera ) :
    Window( width, height, title, flags ),
    camera( std::move( camera ) )
  {
  }

  auto init( ) -> bool override
  {
    if ( !Window::init( ) )
    {
      return false;
    }

    SDL_SetRelativeMouseMode( SDL_FALSE );
    return true;
  }

  auto update( ) -> bool override
  {
    if ( !Window::update( ) )
    {
      return false;
    }

    this->camera->setSize( this->width, this->height );

    // Add your custom event polling and integrate your event system.
    SDL_Event event;

    while ( SDL_PollEvent( &event ) != 0 )
    {
      switch ( event.type )
      {
        case SDL_QUIT: {
          return false;
        }

        case SDL_WINDOWEVENT: {
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

        case SDL_KEYDOWN: {
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

            case SDLK_LSHIFT:
              Key::eLeftShift = true;
              break;

            case SDLK_ESCAPE:
              return false;

            case SDLK_SPACE: {
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

        case SDL_KEYUP: {
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

            case SDLK_LSHIFT:
              Key::eLeftShift = false;
              break;
          }
          break;
        }

        case SDL_MOUSEMOTION: {
          if ( !this->mouseVisible )
          {
            int x;
            int y;
            SDL_GetRelativeMouseState( &x, &y );
            this->camera->processMouse( x, -y );
            break;
          }
        }
      }
    }
    return true;
  }

private:
  std::shared_ptr<Camera> camera;
  bool mouseVisible = true;
};

glm::vec3 getRandomUniquePosition( float min, float max )
{
  static std::vector<glm::vec3> positions;

  static std::random_device rd;
  static std::mt19937 mt( rd( ) );
  static std::uniform_real_distribution<float> dist( min, max );

  glm::vec3 result;

  while ( true )
  {
    result.x = dist( mt );
    result.y = dist( mt );
    result.z = dist( mt );

    bool accepted = true;
    for ( const auto& position : positions )
    {
      if ( result == position )
      {
        accepted = false;
      }
    }

    if ( accepted )
    {
      break;
    }
  };

  return result;
}

class CustomGui : public Gui
{
public:
  CustomGui( RayExec* renderer ) :
    renderer( renderer ) {}

private:
  void configure( ) override
  {
    Gui::configure( );
    ImGui::StyleColorsDark( );
  }

  void render( ) override
  {
    static bool showDemoWindow = false;
    if ( showDemoWindow )
    {
      ImGui::ShowDemoWindow( );
    }

    if ( ImGui::Begin( "Settings" ) )
    {
      //ImGui::SliderFloat( "Speed", &animationSpeed, 0.0F, 2.0F );

      static uint32_t spawnCounter = 0;
      static std::array<char, 10> str;

      if ( ImGui::Button( "Add Box" ) )
      {
        auto cube      = this->renderer->findGeometry( "models/cube.obj" );
        auto transform = glm::translate( glm::mat4( 1.0F ), getRandomUniquePosition( -5.0F, 5.0F ) );
        transform      = glm::scale( transform, glm::vec3( 0.3F, 0.3F, 0.3F ) );

        this->renderer->submitGeometryInstance( instance( cube, transform ) );

        //this->geometryNodes.push_back( box );
      }

      ImGui::SameLine( );

      if ( ImGui::Button( "Add Sphere" ) )
      {
        // "Random Sphere " + std::to_string( spawnCounter++ ),

        auto sphere    = this->renderer->findGeometry( "models/sphere.obj" );
        auto transform = glm::translate( glm::mat4( 1.0F ), getRandomUniquePosition( -5.0F, 5.0F ) );
        transform      = glm::scale( transform, glm::vec3( 0.3F, 0.3F, 0.3F ) );

        this->renderer->submitGeometryInstance( instance( sphere, transform ) );

        //this->geometryNodes.push_back( sphere );
      }

      ImGui::SameLine( );

      if ( ImGui::Button( "Clear scene" ) )
      {
        for ( const auto& node : this->geometryNodes )
        {
          // this->renderer->popNode( node );
        }
      }

      ImGui::Checkbox( "Show ImGui Demo Window", &showDemoWindow );

      auto clearColor = this->renderer->settings.getClearColor( );
      if ( ImGui::ColorEdit4( "##AmbientColor", &clearColor[0] ) )
      {
        this->renderer->settings.setClearColor( clearColor );
      }

      bool rayTrace = this->renderer->settings.getRayTracingEnabled( );
      if ( ImGui::Checkbox( "Toggle Ray Tracing", &rayTrace ) )
      {
        this->renderer->settings.setEnableRayTracing( rayTrace );
        if ( !rayTrace )
        {
          this->renderer->settings.setEnableJitterCam( false );
        }
      }

      if ( rayTrace )
      {
        bool jitterCamEnabled = this->renderer->settings.getJitterCamEnabled( );
        if ( ImGui::Checkbox( "Toggle Jitter Cam", &jitterCamEnabled ) )
        {
          this->renderer->settings.setEnableJitterCam( jitterCamEnabled );
        }

        if ( jitterCamEnabled )
        {
          int jitterCamSampleRate = static_cast<int>( this->renderer->settings.getJitterCamSampleRate( ) );
          if ( ImGui::SliderInt( "Set Jitter Cam Sample Rate", &jitterCamSampleRate, 1, 100 ) )
          {
            this->renderer->settings.setJitterCamSampleRate( jitterCamSampleRate );
          }

          int jitterCamSampleRatePerRayGen = static_cast<int>( this->renderer->settings.getJitterCamSampleRatePerRayGen( ) );
          if ( ImGui::SliderInt( "Set Jitter Cam Sample Rate Per Ray Gen", &jitterCamSampleRatePerRayGen, 1, 10 ) )
          {
            this->renderer->settings.setJitterCamSampleRatePerRayGen( jitterCamSampleRatePerRayGen );
          }
        }

        bool ssaaEnabled = this->renderer->settings.getSsaaEnabled( );
        if ( ImGui::Checkbox( "Toggle SSAA", &ssaaEnabled ) )
        {
          this->renderer->settings.setEnableSsaa( ssaaEnabled );
        }

        if ( ssaaEnabled )
        {
          int ssaaSampleRate = static_cast<int>( this->renderer->settings.getSsaaSampleRate( ) );
          if ( ImGui::SliderInt( "Set SSAA Sample Rate", &ssaaSampleRate, 1, 4 ) )
          {
            this->renderer->settings.setSsaaSampleRate( ssaaSampleRate );
          }
        }

        int depth = static_cast<int>( this->renderer->settings.getRecursionDepth( ) );
        if ( ImGui::SliderInt( "Recursion depth", &depth, 0, 31 ) )
        {
          this->renderer->settings.setRecursionDepth( static_cast<uint32_t>( depth ) );
        }
      }

      const size_t maxFrames = 10000;
      static std::array<float, maxFrames> frameTimes;

      static size_t counter = 0;
      counter               = counter % maxFrames;

      float dt = Time::getDeltaTime( );
      if ( dt > 0.001f )
      {
        if ( counter >= maxFrames - 1 )
        {
          std::fill( frameTimes.begin( ), frameTimes.end( ), 0.0f );
        }

        frameTimes[counter] = dt;
        ++counter;
      }

      ImGui::PlotLines( "Frame Times", frameTimes.data( ), maxFrames, 0, "Frametime", 0.0F, 0.01F, ImVec2( 0.0F, 80.0F ) );
    }

    ImGui::End( );
  }

private:
  RayExec* renderer;
  std::vector<std::shared_ptr<GeometryNode>> geometryNodes;
};

auto main( ) -> int
{
  // Window dimensions.
  const int width  = 1400;
  const int height = 900;

  RayExec renderer;

  // Custom camera
  renderer.setCamera( std::make_shared<CustomCamera>( width, height, glm::vec3( 0.0F, 0.0F, 3.0F ) ) );

  // Custom window
  renderer.setWindow( std::make_shared<CustomWindow>( width, height, "Example", SDL_WINDOW_RESIZABLE, renderer.getCamera( ) ) );

  // Custom ImGui based Gui
  renderer.setGui( std::make_shared<CustomGui>( &renderer ) );

  // Use resources efficiently by introducing the renderer to the anticipated total amount of various entities.
  renderer.settings.setMaxDirectionalLights( 2 );
  renderer.settings.setMaxPointLights( 20 );
  renderer.settings.setMaxGeometryNodes( 100 );

  // ... and initialize the renderer.
  renderer.init( );

  //renderer.setModels( { "models/plane.obj", "models/sphere.obj", "models/awpdlore/awpdlore.obj", "models/cube.obj" } );

  auto awp    = loadObj( "models/awpdlore/awpdlore.obj" );
  auto sphere = loadObj( "models/sphere.obj" );
  auto plane  = loadObj( "models/plane.obj" );
  auto cube   = loadObj( "models/cube.obj" );

  renderer.setGeometries( { awp, sphere, plane, cube } );

  GeometryInstance awpInstance   = instance( awp );
  GeometryInstance planeInstance = instance( plane );

  renderer.setGeometryInstances( { awpInstance, planeInstance } );

  /*
  // Setup the scene
  auto dragonLore            = std::make_shared<GeometryNode>( "Dragon Lore 1", "models/awpdlore/awpdlore.obj" );
  dragonLore->worldTransform = glm::scale( dragonLore->worldTransform, glm::vec3( 0.25F ) );
  dragonLore->worldTransform = glm::rotate( dragonLore->worldTransform, glm::radians( 45.0F ), glm::vec3( 0.0F, 1.0F, 0.0F ) );
  dragonLore->worldTransform = glm::translate( dragonLore->worldTransform, glm::vec3( 0.0F, -1.0F, 0.5F ) );

  auto dragonLore2            = std::make_shared<GeometryNode>( "Dragon Lore 2", "models/awpdlore/awpdlore.obj" );
  dragonLore2->worldTransform = glm::scale( dragonLore2->worldTransform, glm::vec3( 0.25F ) );
  dragonLore2->worldTransform = glm::rotate( dragonLore2->worldTransform, glm::radians( 90.0F ), glm::vec3( 0.0F, 1.0F, 0.0F ) );
  dragonLore2->worldTransform = glm::translate( dragonLore2->worldTransform, glm::vec3( 1.0F, 2.0F, 0.0F ) );

  auto floor = std::make_shared<GeometryNode>( "Floor", "models/plane.obj" );

  auto directionalLight = std::make_shared<DirectionalLightNode>( "Directional Light 1" );

  // Add the model to the renderer. This way they will be queued for rendering.
  renderer.pushNode( dragonLore );
  renderer.pushNode( dragonLore2 );
  renderer.pushNode( floor );
  renderer.pushNode( directionalLight );
  */

  while ( renderer.isRunning( ) )
  {
    //dragonLore->worldTransform = glm::rotate( dragonLore->worldTransform, glm::radians( 90.0F ) * Time::getDeltaTime( ) * animationSpeed, glm::vec3( 0.0F, 1.0F, 0.0F ) );
    renderer.run( );
  }

  return 0;
}
