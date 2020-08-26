#include "Renderer.hpp"
#include <filesystem>

namespace rx
{
  Renderer::Renderer( ) :
    m_window( std::make_shared<WindowBase>( ) ),
    m_camera( std::make_shared<CameraBase>( m_window->getSize( ) ) ),
    m_api( m_window, m_camera ),
    m_initialized( false ),
    m_running( true )
  { }

  Renderer::Renderer( std::shared_ptr<CameraBase> camera ) :
    m_window( std::make_shared<WindowBase>( ) ),
    m_camera( camera ),
    m_api( m_window, m_camera ),
    m_initialized( false ),
    m_running( true )
  { }

  Renderer::Renderer( std::shared_ptr<WindowBase> window ) :
    m_window( window ),
    m_camera( std::make_shared<CameraBase>( m_window->getSize( ) ) ),
    m_api( window, m_camera ),
    m_initialized( false ),
    m_running( true )
  { }

  Renderer::Renderer( std::shared_ptr<WindowBase> window, std::shared_ptr<CameraBase> camera ) :
    m_window( window ),
    m_camera( camera ),
    m_api( m_window, m_camera ),
    m_initialized( false ),
    m_running( true )
  { }

  void Renderer::init( )
  {
    g_window = m_window;

    if ( m_initialized )
      return;

  #ifdef RX_DEBUG
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

    m_running = m_window->render( );
    m_api.render( );
    m_api.update( );
  }

  void Renderer::pushNode( const std::shared_ptr<Node> node )
  {
    m_api.pushNode( node );
  }

  void Renderer::setNodes( const std::vector<std::shared_ptr<Node>>& nodes )
  {
    m_api.setNodes( nodes );
  }

  void Renderer::setCamera( std::shared_ptr<CameraBase> camera )
  {
    m_camera = camera;
  }
  
  void Renderer::setGui( std::shared_ptr<GuiBase> gui )
  {
    m_api.setGui( gui );
    
    if ( m_initialized )
      m_api.initGui( );
  }
}