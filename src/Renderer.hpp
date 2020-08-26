#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Api.hpp"
#include "Base.hpp"

// This is the core of the renderer.
// Only include this file in your application.

namespace rx
{
  class Renderer
  {
  public:
    RX_API Renderer( );
    RX_API Renderer( std::shared_ptr<CameraBase> camera );
    RX_API Renderer( std::shared_ptr<WindowBase> window );
    RX_API Renderer( std::shared_ptr<WindowBase> window, std::shared_ptr<CameraBase> camera );

    RX_API void init( );
    RX_API void run( );

    RX_API bool isRunning( ) { return m_running; }

    RX_API inline const std::shared_ptr<WindowBase> getWindow( ) const { return m_window; }

    RX_API void pushNode( const std::shared_ptr<Node> node );
    RX_API void setNodes( const std::vector<std::shared_ptr<Node>>& nodes );

    RX_API void setCamera( std::shared_ptr<CameraBase> camera );
    RX_API void setGui( std::shared_ptr<GuiBase> gui );

  private:
    std::shared_ptr<WindowBase> m_window;
    std::shared_ptr<CameraBase> m_camera;
    Api m_api;

    bool m_initialized;
    bool m_running;
  };
}

#endif // RENDERER_HPP