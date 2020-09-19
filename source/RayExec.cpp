#include "RayExec.hpp"

namespace RENDERER_NAMESPACE
{
  RayExec::RayExec( ) :
    window( std::make_shared<Window>( ) ),
    camera( std::make_shared<Camera>( this->window->getWidth( ), this->window->getHeight( ) ) ),
    api( this->window, this->camera )
  { }

  RayExec::RayExec( std::shared_ptr<Camera> camera ) :
    window( std::make_shared<Window>( ) ),
    camera( camera ),
    api( this->window, this->camera )
  { }

  RayExec::RayExec( std::shared_ptr<Window> window ) :
    window( window ),
    camera( std::make_shared<Camera>( this->window->getWidth( ), this->window->getHeight( ) ) ),
    api( window, this->camera )
  { }

  RayExec::RayExec( std::shared_ptr<Window> window, std::shared_ptr<Camera> camera ) :
    window( window ),
    camera( camera ),
    api( this->window, this->camera )
  { }

  void RayExec::init( )
  {
    this->api.settings = &this->settings;

    if ( this->settings.getResourcePath( ).empty( ) )
    {
      RX_WARN( "Path to resources was not set. Use Settings::setResourcePath(argc, argv) or Settings::setResourcePath(path) to set it." );
      this->running = false;
      return;
    }

    g_window = this->window;

    if ( this->initialized )
      return;

  #ifdef RX_COPY_RESOURCES
    RX_INFO( "Copying resources to binary output directory. " );

    std::filesystem::copy( RX_RESOURCES_PATH "shaders", RX_PATH_TO_LIBRARY "shaders", std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive );
    std::filesystem::copy( RX_RESOURCES_PATH "models", RX_PATH_TO_LIBRARY "models", std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive );
    std::filesystem::copy( RX_RESOURCES_PATH "textures", RX_PATH_TO_LIBRARY "textures", std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive );
  #endif

    this->window->init( );
    this->api.init( );

    this->initialized = true;
  }

  void RayExec::run( )
  {
    if ( !this->running || !this->initialized )
      return;

    this->running = this->window->update( );
    this->camera->update( );

    if ( !this->running || !this->initialized )
      return;

    this->api.render( );
    this->api.update( );
  }

  void RayExec::setCamera( std::shared_ptr<Camera> camera )
  {
    this->camera = camera;
  }
  
  void RayExec::setGui( std::shared_ptr<Gui> gui )
  {
    this->initialized ? this->api.setGui( gui, true ) : this->api.setGui( gui );
  }
}