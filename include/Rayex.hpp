#ifndef RAYEX_HPP
#define RAYEX_HPP

#include "api/Api.hpp"

/// The rx namespace encapsulates every single component provided by the renderer.
namespace RAYEX_NAMESPACE
{
  /// The main user interface.
  ///
  /// This class provides everything to set up a main loop and fill the scene with geometry and light sources.
  /// Furthermore, the renderer's settings can be changed to better fit the user's purpose.
  /// ### Example
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
  /// // The following example renders a cube in less than 10 lines of code.
  /// Rayex myRenderer;
  /// myRenderer.init( );
  ///
  /// myRenderer.pushNode( std::make_shared<GeometryNode>( "models/cube.obj", Material( "textures/metal.png" ) ) );
  ///
  /// while ( myRenderer.isRunning( ) )
  /// {
  ///   myRenderer.run( );
  /// }
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /// @ingroup BASE
  class RX_API Rayex
  {
  public:
    /// Initializes the renderer.
    ///
    /// This function initializes the window subcomponent as well as the API.
    void init( );

    /// A single function to execute all subcomponents in order.
    ///
    /// This function updates the window and the camera components and calls the update and render functions of the API.
    /// @see RAYEX_NAMESPACE::Camera::update(), RAYEX_NAMESPACE::Window::update(), RAYEX_NAMESPACE::Api::update(), RAYEX_NAMESPACE::Api::render()
    void run( );

    /// @return Returns true if the application is still running and false if the application has stopped.
    auto isRunning( ) const -> bool;

    /// Used to set a custom camera.
    /// @param camera A pointer to a RAYEX_NAMESPACE::Camera object.
    void setCamera( std::shared_ptr<Camera> camera );

    /// Used to set a custom window.
    /// @param window A pointer to a RAYEX_NAMESPACE::Window object.
    void setWindow( std::shared_ptr<Window> window );

    /// Used to set a custom GUI.
    ///
    /// The GUI can be changed multiple times. Even during runtime.
    /// @param gui A pointer to a RAYEX_NAMESPACE::Gui object.
    void setGui( std::shared_ptr<Gui> gui );

    /// @return Returns a pointer to the renderer's window.
    auto getWindow( ) const -> std::shared_ptr<Window> { return _window; }

    /// @return Returns a pointer to the renderer's camera.
    auto getCamera( ) const -> std::shared_ptr<Camera> { return _camera; }

    /// Used to modify any interal rendering settings.
    /// @return Returns the settings.
    auto settings( ) -> Settings& { return _settings; }

    /// Used to access the scene directly by adding or removing elements.
    /// @return Returns the scene.
    auto scene( ) -> Scene& { return _scene; }

  private:
    std::shared_ptr<Window> _window = nullptr;                  ///< The window used to create a surface from.
    std::shared_ptr<Camera> _camera = nullptr;                  ///< The camera that will be used for rendering.
    std::shared_ptr<Gui> _gui       = nullptr;                  ///< The ImGui-based GUI.
    std::unique_ptr<Api> _api       = std::make_unique<Api>( ); ///< Manages Vulkan-related tasks.

    bool _initialized = false; ///< Keeps track of the initialization status.
    bool _running     = true;  ///< Keeps track of whether or not the main loop should still be continued.
    bool _initScene   = true;  ///< Keeps track of whether or not to initialize the scene.

    Settings _settings = { }; ///< Stores all of rayexe's settings.
    Scene _scene       = { }; ///< Contains all meshes, all mesh instances and all light sources.
  };
} // namespace RAYEX_NAMESPACE

#endif // RAYEX_HPP
