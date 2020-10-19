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

    this->api->window   = this->window;
    this->api->camera   = this->camera;
    this->api->settings = &this->settings;
    g_window            = this->window;

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

  void RayExec::submitGeometryInstance( std::shared_ptr<GeometryInstance> geometryInstance )
  {
    uint32_t limit = this->settings.maxGeometryInstances.has_value( ) ? this->settings.maxGeometryInstances.value( ) : g_maxGeometryInstances;
    if ( this->api->scene.geometryInstances.size( ) >= limit )
    {
      RX_ERROR( "Failed to submit geometry instance because instance buffer size has been exceeded. To avoid this error, increase the amount of supported geometry instances using RAYEXEC_NAMESPACE::RayExec::Settings::setMaxGeometryInstances(uint32_t)." );
      return;
    }

    this->api->scene.geometryInstances.push_back( geometryInstance );
    this->api->uploadGeometryInstancesToBuffer = true;
  }

  void RayExec::setGeometryInstances( const std::vector<std::shared_ptr<GeometryInstance>>& geometryInstances )
  {
    uint32_t limit = this->settings.maxGeometryInstances.has_value( ) ? this->settings.maxGeometryInstances.value( ) : g_maxGeometryInstances;
    if ( this->api->scene.geometryInstances.size( ) >= limit )
    {
      RX_ERROR( "Failed to set geometry instances because instance buffer size has been exceeded. To avoid this error, increase the amount of supported geometry instances using RAYEXEC_NAMESPACE::RayExec::Settings::setMaxGeometryInstances(uint32_t)." );
      return;
    }

    this->api->scene.geometryInstances         = geometryInstances;
    this->api->uploadGeometryInstancesToBuffer = true;
  }

  void RayExec::submitDirectionalLight( std::shared_ptr<DirectionalLight> light )
  {
    uint32_t limit = this->settings.maxDirectionalLights.has_value( ) ? this->settings.maxDirectionalLights.value( ) : g_maxDirectionalLights;
    if ( this->api->scene.directionalLights.size( ) >= limit )
    {
      RX_ERROR( "Failed to submit directional light because buffer size has been exceeded. To avoid this error, increase the amount of supported geometry instances using RAYEXEC_NAMESPACE::RayExec::Settings::setMaxDirectionalLights(uint32_t)." );
      return;
    }

    this->api->scene.directionalLights.push_back( light );
  }

  void RayExec::removeGeometryInstance( std::shared_ptr<GeometryInstance> geometryInstance )
  {
    RX_ASSERT( geometryInstance != nullptr, "An invalid geometry instance can not be removed." );

    std::vector<std::shared_ptr<GeometryInstance>> temp( this->api->scene.geometryInstances );
    this->api->scene.geometryInstances.clear( );
    this->api->scene.geometryInstances.reserve( temp.size( ) );

    for ( auto it : temp )
    {
      if ( it != geometryInstance )
      {
        this->api->scene.geometryInstances.push_back( it );
      }
    }

    this->api->uploadGeometryInstancesToBuffer = true;
  }

  void RayExec::submitGeometry( std::shared_ptr<Geometry> geometry )
  {
    this->api->scene.geometries.push_back( geometry );
  }

  void RayExec::setGeometries( const std::vector<std::shared_ptr<Geometry>>& geometries )
  {
    this->api->scene.geometries = geometries;
  }

  auto RayExec::findGeometry( std::string_view path ) const -> std::shared_ptr<Geometry>
  {
    for ( std::shared_ptr<Geometry> geometry : this->api->scene.geometries )
    {
      if ( geometry->path == path )
      {
        return geometry;
      }
    }

    RX_INFO( "Could not find geometry in scene. Trying to create geometry instead." );
    return loadObj( path );
  }

} // namespace RAYEXEC_NAMESPACE
