#include "Rayex.hpp"

#include <utility>

namespace RAYEX_NAMESPACE
{
  void Rayex::init( )
  {
    RX_INFO( "Starting Rayex." );

    if ( _window == nullptr )
    {
      RX_VERBOSE( "No custom window implementation was provided. Using default implementation instead." );
      _window = std::make_shared<Window>( );
    }

    if ( _api._scene._currentCamera == nullptr )
    {
      RX_VERBOSE( "No custom camera implementation was provided. Using default implementation instead." );
      _api._scene._currentCamera = std::make_shared<Camera>( _window->getWidth( ), _window->getHeight( ) );
      _api._scene._cameras.insert( _api._scene._currentCamera );
    }

    _api._window          = _window;
    _api._camera          = _api._scene._currentCamera;
    _api._scene._settings = &_api._settings;

    if ( _api._settings.getAssetsPath( ).empty( ) )
    {
      _api._settings.setDefaultAssetsPath( );
    }

    if ( _initialized )
    {
      RX_ERROR( "Renderer was already initialized." );
      return;
    }

#ifdef RX_COPY_ASSETS
    RX_INFO( "Copying resources to binary output directory. " );

    std::filesystem::copy( RX_ASSETS_PATH "shaders", RX_PATH_TO_LIBRARY "shaders", std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive );
    std::filesystem::copy( RX_ASSETS_PATH "models", RX_PATH_TO_LIBRARY "models", std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive );
#endif

    _initialized = _window->init( );
    _api.initBase( );
  }

  auto Rayex::isRunning( ) const -> bool
  {
    if ( !_running )
    {
      // Print results of the session.
      rx::Time::benchmark( );

      RX_INFO( "Shutting down Rayex." );
    }

    return _running;
  }

  void Rayex::run( )
  {
    if ( !_running || !_initialized )
    {
      return;
    }

    static bool firstRun = true;
    if ( _initScene )
    {
      _api.initScene( );
      _initScene = false;
    }
    else
    {
      if ( firstRun )
      {
        RX_ERROR( "Failed to initialize scene. Did you forget to call setModels(std::vector<std::string>)?" );
        _running = false;
        return;
      }
    }

    firstRun = false;

    _running = _window->update( );
    _api._scene._currentCamera->update( );

    _api.render( );
  }

  void Rayex::setWindow( std::shared_ptr<Window> window )
  {
    _window      = window;
    _api._window = _window;
  }

  void Rayex::setGui( std::shared_ptr<Gui> gui )
  {
    _initialized ? _api.setGui( gui, true ) : _api.setGui( gui );
  }

  void Rayex::reset( )
  {
    rx::components::geometryIndex = 0;
    rx::components::textureIndex  = 0;

    _api._scene._deleteTextures = true;

    //components::device.waitIdle( );
    //_api._scene.prepareBuffers( );
  }
} // namespace RAYEX_NAMESPACE
