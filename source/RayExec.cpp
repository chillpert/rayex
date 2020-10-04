#include "RayExec.hpp"

#include <utility>

namespace RAYEXEC_NAMESPACE
{
  RayExec::RayExec( ) :
    api( std::make_unique<Api>( ) ) {}

  void RayExec::init( )
  {
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

    this->api->window   = this->window;
    this->api->camera   = this->camera;
    this->api->settings = &this->settings;
    g_window            = this->window;

    if ( this->settings.getResourcePath( ).empty( ) )
    {
      RX_WARN( "Path to resources was not set. Use Settings::setResourcePath(argc, argv) or Settings::setResourcePath(path) to set it. Closing application." );
      this->running = false;
      return;
    }

    if ( this->initialized )
    {
      return;
    }

#ifdef RX_COPY_RESOURCES
    RX_INFO( "Copying resources to binary output directory. " );

    std::filesystem::copy( RX_RESOURCES_PATH "shaders", RX_PATH_TO_LIBRARY "shaders", std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive );
    std::filesystem::copy( RX_RESOURCES_PATH "models", RX_PATH_TO_LIBRARY "models", std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive );
    std::filesystem::copy( RX_RESOURCES_PATH "textures", RX_PATH_TO_LIBRARY "textures", std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive );
#endif

    this->initialized = this->window->init( );
    this->api->initBase( );
  }

  void RayExec::run( )
  {
    if ( this->initScene )
    {
      this->api->initScene( );
      this->initScene = false;
    }

    if ( !this->running || !this->initialized )
    {
      return;
    }

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

  void RayExec::setModels( const std::vector<std::string>& modelPaths )
  {
    this->api->setModels( modelPaths );
    this->initScene = true;
  }

  void RayExec::popNode( std::shared_ptr<Node> node )
  {
    this->api->popNode( node );
  }
} // namespace RAYEXEC_NAMESPACE