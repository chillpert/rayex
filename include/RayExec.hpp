#ifndef RAYEXEC_HPP
#define RAYEXEC_HPP

#include "api/Api.hpp"
#include "base/Base.hpp"

/// The rx namespace encapsulates every single component provided by the renderer.
namespace RAYEXEC_NAMESPACE
{
  /// The main user interface.
  ///
  /// This class provides everything to set up a main loop and fill the scene with geometry and light sources.
  /// ### Example
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
  /// // The following example renders a cube in less than 10 lines of code.
  /// RayExecmyRenderer;
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
  /// @todo Not calling init will cause a segmentation fault as the application still tries to call device.waitIdle on a device that was not init.
  class RayExec
  {
  public:
    RX_API RayExec( );

    /// @param camera A pointer to a camera object specified by the client.
    RX_API RayExec( std::shared_ptr<Camera> camera );

    /// @param window A pointer to a window object specified by the client.
    RX_API RayExec( std::shared_ptr<Window> window );

    /// @param window A pointer to a window object specified by the client.
    /// @param camera A pointer to a camera object specified by the client.
    RX_API RayExec( std::shared_ptr<Window> window, std::shared_ptr<Camera> camera );

    /// Initializes the renderer.
    ///
    /// This function initializes the window subcomponent as well as the API.
    /// Furthermore, it copies all shaders, textures and models to the executables directory to avoid issues with full and relative paths.
    /// @todo The copying process is temporary for now and has to be changed for a release build.
    RX_API void init( );

    /// A single function to execute all subcomponents.
    ///
    /// This function updates the window and the camera components and calls the update and render functions of the API.
    /// @see RAYEXEC_NAMESPACE::Camera::update(), RAYEXEC_NAMESPACE::Window::update(), RAYEXEC_NAMESPACE::Api::update(), RAYEXEC_NAMESPACE::Api::render()
    RX_API void run( );

    /// @return Returns true if the application is still running and false if the application has stopped.
    RX_API bool isRunning( ) { return running; }

    /// @return Returns a pointer to the renderer's window.
    RX_API inline const std::shared_ptr<Window> getWindow( ) const { return window; }

    /// @return Returns a pointer to the renderer's camera.
    RX_API inline const std::shared_ptr<Camera> getCamera( ) const { return camera; }

    /// Used to add another arbitrary node to the scene.
    /// @param node The node to add.
    /// @see RAYEXEC_NAMESPACE::Api::pushNode()
    template <typename T = Model>
    void pushNode( const std::shared_ptr<Node> node )
    {
      api.pushNode<T>( node );
    }

    RX_API void popNode( const std::shared_ptr<Node> node );

    /// Used to overwrite the entire scene with new nodes.
    /// @param nodes A vector of nodes describing the new scene.
    /// @see RAYEXEC_NAMESPACE::Api::setNodes()
    template <typename T = Model>
    void setNodes( const std::vector<std::shared_ptr<Node>>& nodes )
    {
      api.setNodes<T>( nodes );
    }

    /// Used to set the renderer's camera.
    /// @param camera The camera the renderer should be using.
    RX_API void setCamera( std::shared_ptr<Camera> camera );

    /// Used to set the renderer's GUI.
    /// @param gui The GUI the renderer should be using.
    RX_API void setGui( std::shared_ptr<Gui> gui );

    RX_API void setModels( const std::vector<std::string>& modelPaths );

    Settings settings;

  private:
    std::shared_ptr<Window> window; ///< A pointer to a RAYEXEC_NAMESPACE::Window object.
    std::shared_ptr<Camera> camera; ///< A pointer to a RAYEXEC_NAMESPACE::Camera object.
    Api api;                        ///< Contains all Vulkan related components.

    bool initialized = false; ///< Keeps track of the initialization status.
    bool running     = true;  ///< Keeps track of whether or not the main loop should still be continued.
  };
} // namespace RAYEXEC_NAMESPACE

#endif // RAYEXEC_HPP