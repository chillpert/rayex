#ifndef RAYEXEC_HPP
#define RAYEXEC_HPP

#include "api/Api.hpp"

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
    RX_API auto isRunning( ) const -> bool;

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

    /// Used to submit a geometry instance for rendering.
    /// @param geometryInstance The instance to queue for rendering.
    /// @note This function does not invoke any draw calls.
    RX_API void submitGeometryInstance( std::shared_ptr<GeometryInstance> geometryInstance );

    /// Used to submit multiple geometry instances for rendering, replacing all existing instances.
    /// @param geometryInstances The instances to queue for rendering.
    /// @note This function does not invoke any draw calls.
    RX_API void setGeometryInstances( const std::vector<std::shared_ptr<GeometryInstance>>& geometryInstances );

    RX_API void submitDirectionalLight( std::shared_ptr<DirectionalLight> light );

    /// Used to remove a geometry instance.
    ///
    /// Once a geometry instance was removed, it will no longer be rendered.
    /// @param geometryInstance The instance to remove.
    RX_API void removeGeometryInstance( std::shared_ptr<GeometryInstance> geometryInstance );

    /// Used to submit a geometry and set up its buffers.
    ///
    /// Once a geometry was submitted, geometry instances referencing this particular geometry can be drawn.
    /// @param geometry The geometry to submit.
    RX_API void submitGeometry( std::shared_ptr<Geometry> geometry );

    /// Used to submit multiple geometries and set up their buffers.
    ///
    /// Once a geometry was submitted, geometry instances referencing this particular geometry can be drawn.
    /// @param geometries The geometries to submit.
    RX_API void setGeometries( const std::vector<std::shared_ptr<Geometry>>& geometries );

    /// Used to retrieve a geoemtry based on its path.
    /// @param path The geometry's model's path, relative to the path to assets.
    RX_API auto findGeometry( std::string_view path ) const -> std::shared_ptr<Geometry>;

    /// @return Returns a pointer to the renderer's window.
    [[nodiscard]] RX_API inline auto getWindow( ) const -> std::shared_ptr<Window> { return this->window; }

    /// @return Returns a pointer to the renderer's camera.
    [[nodiscard]] RX_API inline auto getCamera( ) const -> std::shared_ptr<Camera> { return this->camera; }

    /// Used to modify any interal rendering settings.
    /// @see RAYEXEC_NAMESPACE::Settings
    Settings settings;

  private:
    std::shared_ptr<Window> window = nullptr;                  ///< The window used to create a surface from.
    std::shared_ptr<Camera> camera = nullptr;                  ///< The camera that will be used for rendering.
    std::shared_ptr<Gui> gui       = nullptr;                  ///< The ImGui-based GUI.
    std::unique_ptr<Api> api       = std::make_unique<Api>( ); ///< Manages Vulkan-related tasks.

    bool initialized = false; ///< Keeps track of the initialization status.
    bool running     = true;  ///< Keeps track of whether or not the main loop should still be continued.
    bool initScene   = true;  ///< Keeps track of whether or not to initialize the scene.
  };
} // namespace RAYEXEC_NAMESPACE

#endif // RAYEXEC_HPP
