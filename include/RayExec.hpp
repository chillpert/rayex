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
  /// Furthermore, the renderer's settings can be changed to better fit the user's purpose.
  /// ### Example
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
  /// // The following example renders a cube in less than 10 lines of code.
  /// RayExec myRenderer;
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
  class RayExec
  {
  public:
    RX_API RayExec( );

    /// Initializes the renderer.
    ///
    /// This function initializes the window subcomponent as well as the API.
    RX_API void init( );

    /// A single function to execute all subcomponents in order.
    ///
    /// This function updates the window and the camera components and calls the update and render functions of the API.
    /// @see RAYEXEC_NAMESPACE::Camera::update(), RAYEXEC_NAMESPACE::Window::update(), RAYEXEC_NAMESPACE::Api::update(), RAYEXEC_NAMESPACE::Api::render()
    RX_API void run( );

    /// @return Returns true if the application is still running and false if the application has stopped.
    RX_API auto isRunning( ) -> bool { return running; }

    /// Used to set a custom camera.
    /// @param camera A pointer to a RAYEXEC_NAMESPACE::Camera object.
    RX_API void setCamera( std::shared_ptr<Camera> camera );

    /// Used to set a custom window.
    /// @param window A pointer to a RAYEXEC_NAMESPACE::Window object.
    RX_API void setWindow( std::shared_ptr<Window> window );

    /// Used to set a custom GUI.
    ///
    /// The GUI can be changed multiple times. Even during runtime.
    /// @param gui A pointer to a RAYEXEC_NAMESPACE::Gui object.
    RX_API void setGui( std::shared_ptr<Gui> gui );

    /// Used to set all models that can be rendered and to initialize them.
    ///
    /// Resources can be allocated more efficiently if all possible models are known to the renderer in advance.
    /// @param modelPaths A vector containing paths to model files.
    /// @warning To render any geometry the user has to call this function after init() was called.
    RX_API void setModels( const std::vector<std::string>& modelPaths );

    /// @return Returns a pointer to the renderer's window.
    [[nodiscard]] RX_API inline auto getWindow( ) const -> std::shared_ptr<Window> { return window; }

    /// @return Returns a pointer to the renderer's camera.
    [[nodiscard]] RX_API inline auto getCamera( ) const -> std::shared_ptr<Camera> { return camera; }

    void setRoot( std::shared_ptr<Node> root );

    /// Used to add another arbitrary node to the scene.
    /// @param node A pointer to a RAYEXEC_NAMESPACE::Node object.
    /// @see rx::Api::pushNode() for implementation details.
    template <typename T = Model>
    void pushNode( std::shared_ptr<Node> node )
    {
      api->pushNode<T>( node );
    }

    /// Used to delete a node from the renderer.
    /// @param node A pointer to a RAYEXEC_NAMESPACE::Node object.
    RX_API void popNode( std::shared_ptr<Node> node );

    /// Used to overwrite the entire scene with new nodes at once.
    /// @param nodes A vector of rx::Node objects describing the new scene.
    /// @see rx::Api::setNodes()
    template <typename T = Model>
    void setNodes( const std::vector<std::shared_ptr<Node>>& nodes )
    {
      api->setNodes<T>( nodes );
    }

    Settings settings;

  private:
    std::shared_ptr<Window> window = nullptr; ///< A pointer to a RAYEXEC_NAMESPACE::Window object.
    std::shared_ptr<Camera> camera = nullptr; ///< A pointer to a RAYEXEC_NAMESPACE::Camera object.
    std::shared_ptr<Gui> gui       = nullptr; ///< A pointer to a RAYEXEC_NAMESPACE::Gui object.
    std::unique_ptr<Api> api       = nullptr; ///< Contains all Vulkan related components.

    bool initialized = false; ///< Keeps track of the initialization status.
    bool running     = true;  ///< Keeps track of whether or not the main loop should still be continued.
    bool initScene   = true;  ///< Keeps track of whether or not to initialize the scene.
  };
} // namespace RAYEXEC_NAMESPACE

#endif // RAYEXEC_HPP
