#include "RayExec.hpp"

#include <utility>

namespace RAYEXEC_NAMESPACE
{
  void RayExec::init( )
  {
    RX_INFO( "Starting RayExec." );

    if ( this->window == nullptr )
    {
      RX_VERBOSE( "No custom window implementation was provided. Using default implementation instead." );
      this->window = std::make_shared<Window>( );
    }

    if ( this->camera == nullptr )
    {
      RX_VERBOSE( "No custom camera implementation was provided. Using default implementation instead." );
      this->camera = std::make_shared<Camera>( this->window->getWidth( ), this->window->getHeight( ) );
    }

    this->api->window          = this->window;
    this->api->camera          = this->camera;
    this->api->settings        = &this->settings;
    this->api->scene           = &this->scene;
    this->api->scene->settings = &this->settings;
    g_window                   = this->window;

    if ( this->settings.getAssetsPath( ).empty( ) )
    {
      this->settings.setDefaultAssetsPath( );
    }

    if ( this->initialized )
    {
      RX_ERROR( "Renderer was already initialized." );
      return;
    }

#ifdef RX_COPY_ASSETS
    RX_INFO( "Copying resources to binary output directory. " );

    std::filesystem::copy( RX_ASSETS_PATH "shaders", RX_PATH_TO_LIBRARY "shaders", std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive );
    std::filesystem::copy( RX_ASSETS_PATH "models", RX_PATH_TO_LIBRARY "models", std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive );
    std::filesystem::copy( RX_ASSETS_PATH "textures", RX_PATH_TO_LIBRARY "textures", std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive );
#endif

    this->initialized = this->window->init( );
    this->api->initBase( );
  }

  auto RayExec::isRunning( ) const -> bool
  {
    if ( !this->running )
    {
      // Print results of the session.
      rx::Time::benchmark( );

      RX_INFO( "Shutting down RayExec." );
    }

    return this->running;
  }

  void RayExec::run( )
  {
    if ( !this->running || !this->initialized )
    {
      return;
    }

    static bool firstRun = true;
    if ( this->initScene )
    {
      this->api->initScene( );
      this->initScene = false;
    }
    else
    {
      if ( firstRun )
      {
        RX_ERROR( "Failed to initialize scene. Did you forget to call setModels(std::vector<std::string>)?" );
        this->running = false;
        return;
      }
    }

    firstRun = false;

    this->running = this->window->update( );
    this->camera->update( );

    if ( !this->running )
    {
      return;
    }

    this->running = this->api->render( );

    if ( !this->running )
    {
      return;
    }

    this->api->update( );
  }

  void RayExec::setCamera( std::shared_ptr<Camera> camera )
  {
    this->camera = camera;

    if ( this->api != nullptr )
      this->api->camera = this->camera;
  }

  void RayExec::setWindow( std::shared_ptr<Window> window )
  {
    this->window = window;

    if ( this->api != nullptr )
      this->api->window = this->window;
  }

  void RayExec::setGui( std::shared_ptr<Gui> gui )
  {
    this->initialized ? this->api->setGui( gui, true ) : this->api->setGui( gui );
  }
} // namespace RAYEXEC_NAMESPACE
