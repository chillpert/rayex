#include "CustomCamera.hpp"
#include "CustomGui.hpp"
#include "CustomWindow.hpp"

auto main( ) -> int
{
  // Window dimensions.
  const int width  = 1400;
  const int height = 900;

  rx::RayExec renderer;

  // Custom camera
  renderer.setCamera( std::make_shared<CustomCamera>( width, height, glm::vec3( 0.0F, 0.0F, 3.0F ) ) );

  // Custom window
  renderer.setWindow( std::make_shared<CustomWindow>( width, height, "RayExec Example", SDL_WINDOW_RESIZABLE, renderer.getCamera( ) ) );

  // Custom ImGui based Gui
  renderer.setGui( std::make_shared<CustomGui>( &renderer ) );

  // Use resources efficiently by introducing the renderer to the anticipated total amount of various entities.
  renderer.settings.setMaxDirectionalLights( 2 );
  renderer.settings.setMaxPointLights( 20 );
  renderer.settings.setMaxGeometryInstances( 100 );

  // ... and initialize the renderer.
  renderer.init( );

  // Load geometries.
  auto awp    = rx::loadObj( "models/awpdlore/awpdlore.obj" );
  auto sphere = rx::loadObj( "models/sphere.obj" );
  auto plane  = rx::loadObj( "models/plane.obj" );
  auto cube   = rx::loadObj( "models/cube.obj" );

  // Submit geometries.
  renderer.setGeometries( { awp, sphere, plane, cube } );

  // Create instances of the geometries.
  auto transform = glm::scale( glm::mat4( 1.0F ), glm::vec3( 0.25F ) );
  transform      = glm::rotate( transform, glm::radians( 45.0F ), glm::vec3( 0.0F, 1.0F, 0.0F ) );
  transform      = glm::translate( transform, glm::vec3( 0.0F, -1.0F, 0.5F ) );

  auto awpInstance1 = rx::instance( awp, transform );

  transform = glm::scale( glm::mat4( 1.0F ), glm::vec3( 0.25F ) );
  transform = glm::rotate( transform, glm::radians( 90.0F ), glm::vec3( 0.0F, 1.0F, 0.0F ) );
  transform = glm::translate( transform, glm::vec3( 1.0F, 2.0F, 0.0F ) );

  auto awpInstance2 = rx::instance( awp, transform );

  auto planeInstance = rx::instance( plane );

  // Submit lights.
  auto directionalLight       = std::make_shared<rx::DirectionalLight>( );
  directionalLight->direction = glm::vec3( -4.0F, 10.0F, 5.0F );

  renderer.submitDirectionalLight( directionalLight );

  // Submit instances for drawing.
  renderer.setGeometryInstances( { awpInstance1, planeInstance, awpInstance2 } );

  while ( renderer.isRunning( ) )
  {
    renderer.run( );
  }

  return 0;
}
