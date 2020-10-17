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

    RX_API void submitGeometryInstance( const GeometryInstance& geometryInstance )
    {
      this->api->scene.geometryInstances.push_back( geometryInstance );
      this->api->uploadRayTracingInstancesToBuffer = true;
    }

    RX_API void setGeometryInstances( const std::vector<GeometryInstance>& geometryInstances )
    {
      this->api->scene.geometryInstances           = geometryInstances;
      this->api->uploadRayTracingInstancesToBuffer = true;
    }

    RX_API void submitGeometry( std::shared_ptr<Geometry> geometry )
    {
      this->api->scene.geometries.push_back( geometry );
    }

    RX_API void setGeometries( const std::vector<std::shared_ptr<Geometry>>& geometries )
    {
      this->api->scene.geometries = geometries;
    }

    RX_API std::shared_ptr<Geometry> findGeometry( std::string_view path )
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

    /// @return Returns a pointer to the renderer's window.
    [[nodiscard]] RX_API inline auto getWindow( ) const -> std::shared_ptr<Window> { return window; }

    /// @return Returns a pointer to the renderer's camera.
    [[nodiscard]] RX_API inline auto getCamera( ) const -> std::shared_ptr<Camera> { return camera; }

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
