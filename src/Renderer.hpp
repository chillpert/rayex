#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Api.hpp"
#include "Base.hpp"

/// The rx namespace encapsulates every single component provided by the renderer.
namespace rx
{
  /// The main user interface.
  /// 
  /// This class provides everything to set up a main loop and fill the scene with geometry and light sources.
  /// ### Example
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
  /// // The following example renders a cube in less than 10 lines of code.
  /// Renderer myRenderer;
  /// myRenderer.init( );
  /// 
  /// myRenderer.pushNode( std::make_shared<GeometryNode>( "models/cube.obj", Material( "textures/metal.png" ) ) );
  /// 
  /// while ( myRenderer.isRunning( ) )
  /// {
  ///   myRenderer.run( );
  /// }
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /// @ingroup Base
  class Renderer
  {
  public:
    RX_API Renderer( );

    /// @param camera A pointer to a camera object specified by the client.
    RX_API Renderer( std::shared_ptr<CameraBase> camera );

    /// @param window A pointer to a window object specified by the client.
    RX_API Renderer( std::shared_ptr<WindowBase> window );

    /// @param window A pointer to a window object specified by the client.
    /// @param camera A pointer to a camera object specified by the client.
    RX_API Renderer( std::shared_ptr<WindowBase> window, std::shared_ptr<CameraBase> camera );

    /// Initializes the renderer.
    /// 
    /// This function initializes the window subcomponent as well as the API.
    /// Furthermore, it copies all shaders, textures and models to the executables directory to avoid issues with full and relative paths.
    /// @todo The copying process is temporary for now and has to be changed for a release build.
    RX_API void init( );

    /// A single function to execute all subcomponents.
    /// 
    /// This function updates the window and the camera components and calls the update and render functions of the API.
    /// @see rx::CameraBase::update(), rx::WindowBase::update(), rx::Api::update(), rx::Api::render()
    RX_API void run( );

    /// @return Returns true if the application is still running and false if the application has stopped.
    RX_API bool isRunning( ) { return m_running; }

    /// @return Returns a pointer to the renderer's window.
    RX_API inline const std::shared_ptr<WindowBase> getWindow( ) const { return m_window; }
    
    /// @return Returns a pointer to the renderer's camera.
    RX_API inline const std::shared_ptr<CameraBase> getCamera( ) const { return m_camera; }

    /// Used to add another arbitrary node to the scene.
    /// @param node The node to add.
    /// @see rx::Api::pushNode()
    template <typename T = Model>
    RX_API void pushNode( const std::shared_ptr<Node> node )
    {
      m_api.pushNode<T>( node );
    }

    /// Used to overwrite the entire scene with new nodes.
    /// @param nodes A vector of nodes describing the new scene.
    /// @see rx::Api::setNodes()
    template <typename T = Model>
    RX_API void setNodes( const std::vector<std::shared_ptr<Node>>& nodes )
    {
      m_api.setNodes<T>( node );
    }

    /// Used to set the renderer's camera.
    /// @param camera The camera the renderer should be using.
    RX_API void setCamera( std::shared_ptr<CameraBase> camera );

    /// Used to set the renderer's GUI.
    /// @param gui The GUI the renderer should be using.
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