#include "Renderer.hpp"

namespace RENDERER_NAMESPACE
{
  Renderer::Renderer( ) :
    m_window( std::make_shared<Window>( ) ),
    m_camera( std::make_shared<Camera>( m_window->getWidth( ), m_window->getHeight( ) ) ),
    m_api( m_window, m_camera )
  { }

  Renderer::Renderer( std::shared_ptr<Camera> camera ) :
    m_window( std::make_shared<Window>( ) ),
    m_camera( camera ),
    m_api( m_window, m_camera )
  { }

  Renderer::Renderer( std::shared_ptr<Window> window ) :
    m_window( window ),
    m_camera( std::make_shared<Camera>( m_window->getWidth( ), m_window->getHeight( ) ) ),
    m_api( window, m_camera )
  { }

  Renderer::Renderer( std::shared_ptr<Window> window, std::shared_ptr<Camera> camera ) :
    m_window( window ),
    m_camera( camera ),
    m_api( m_window, m_camera )
  { }

  void Renderer::init( )
  {
    m_api.m_settings = &m_settings;

    if ( m_settings.getResourcePath( ).empty( ) )
    {
      RX_WARN( "Path to resources was not set. Use Settings::setResourcePath(argc, argv) or Settings::setResourcePath(path) to set it." );
      m_running = false;
      return;
    }

    g_window = m_window;

    if ( m_initialized )
      return;

  #ifdef RX_COPY_RESOURCES
    std::filesystem::copy( RX_RESOURCES_PATH "shaders", RX_PATH_TO_LIBRARY "shaders", std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive );
    std::filesystem::copy( RX_RESOURCES_PATH "models", RX_PATH_TO_LIBRARY "models", std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive );
    std::filesystem::copy( RX_RESOURCES_PATH "textures", RX_PATH_TO_LIBRARY "textures", std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive );
  #endif

    m_window->init( );
    m_api.init( );

    m_initialized = true;
  }

  void Renderer::run( )
  {
    if ( !m_running || !m_initialized )
      return;

    m_running = m_window->update( );
    m_camera->update( );

    if ( !m_running || !m_initialized )
      return;

    m_api.render( );
    m_api.update( );
  }

  void Renderer::setCamera( std::shared_ptr<Camera> camera )
  {
    m_camera = camera;
  }
  
  void Renderer::setGui( std::shared_ptr<Gui> gui )
  {
    m_initialized ? m_api.setGui( gui, true ) : m_api.setGui( gui );
  }
}