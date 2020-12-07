#include "CustomCamera.hpp"
#include "CustomGui.hpp"
#include "CustomWindow.hpp"

int main( )
{
  // Window dimensions.
  const int width  = 1280;
  const int height = 720;

  rx::Rayex renderer;

  // Custom camera
  renderer.scene.setCamera( std::make_shared<CustomCamera>( width, height, glm::vec3( 0.0F, 0.0F, 3.0F ) ) );

  // Custom window
  renderer.setWindow( std::make_shared<CustomWindow>( width, height, "Rayex Example", SDL_WINDOW_RESIZABLE, &renderer.scene ) );

  // Custom ImGui based Gui
  renderer.setGui( std::make_shared<CustomGui>( &renderer ) );

  // Avoid buffer recreations by setting an upper limit to geometries and geometry instances.
  renderer.settings.setMaxGeometryInstances( 1000 );
  renderer.settings.setMaxGeoemtry( 8 );

  // ... and initialize the renderer.
  renderer.init( );

  /*
  // Load geometries.
  auto awp     = rx::loadObj( "models/awpdlore/awpdlore.obj" );
  auto plane   = rx::loadObj( "models/plane.obj" );
  auto cornell = rx::loadObj( "models/CornellBox.obj" );
  // Set lamp submesh emittance.
  cornell->meshes[7].material.emission = glm::vec3( 10.0F );

  rx::Material customMaterial;
  customMaterial.diffuseTexPath = "models/metal.png";
  customMaterial.emission       = glm::vec3( 1.0F );
  plane->setMaterial( customMaterial );

  // Submit geometries.
  renderer.scene.setGeometries( { awp, plane, cornell } );

  // Create instances of the geometries.
  auto transform = glm::scale( glm::mat4( 1.0F ), glm::vec3( 0.25F ) );
  transform      = glm::rotate( transform, glm::radians( 45.0F ), glm::vec3( 0.0F, 1.0F, 0.0F ) );
  transform      = glm::translate( transform, glm::vec3( 0.0F, -1.0F, 0.5F ) );

  auto awpInstance1 = rx::instance( awp, transform );

  transform = glm::scale( glm::mat4( 1.0F ), glm::vec3( 0.25F ) );
  transform = glm::rotate( transform, glm::radians( 90.0F ), glm::vec3( 0.0F, 1.0F, 0.0F ) );
  transform = glm::translate( transform, glm::vec3( 1.0F, 2.0F, 0.0F ) );

  auto awpInstance2 = rx::instance( awp, transform );

  transform = glm::scale( glm::mat4( 1.0F ), glm::vec3( 1.0F ) );
  transform = glm::translate( transform, glm::vec3( 0.0F, -80.0F, 0.0F ) );

  auto planeInstance = rx::instance( plane, transform );

  transform            = glm::translate( glm::mat4( 1.0F ), glm::vec3( -5.0F, 0.0F, 0.0F ) );
  auto cornellInstance = rx::instance( cornell, transform );

  // Submit instances for drawing.
  renderer.scene.setGeometryInstances( { awpInstance1, awpInstance2, cornellInstance, planeInstance } );

  // Submit lights.
  auto directionalLight = rx::directionalLightInstance( glm::vec3( -4.0F, 10.0F, 5.0F ) );

  renderer.scene.submitDirectionalLight( directionalLight );

  renderer.scene.setEnvironmentMap( "models/skybox/cubemap_yokohama_rgba.ktx" );
  */

  auto cornell = rx::loadObj( "models/CornellBox.obj" );
  // Set lamp submesh emittance.
  cornell->meshes[7].material.emission = glm::vec3( 10.0F );

  renderer.scene.submitGeometry( cornell );

  auto transform       = glm::translate( glm::mat4( 1.0F ), glm::vec3( 0.75F, -1.0F, -1.0F ) );
  auto cornellInstance = rx::instance( cornell, transform );

  renderer.scene.submitGeometryInstance( cornellInstance );

  while ( renderer.isRunning( ) )
  {
    renderer.run( );

    /*
    if ( awpInstance1 != nullptr )
    {
      awpInstance1->setTransform( glm::rotate( awpInstance1->transform, rx::Time::getDeltaTime( ) * 0.5F, glm::vec3( 0.0F, 1.0F, 0.0F ) ) );
    }
    */

    // Extra tests for memcpy error: (hold to spawn many boxes at once)
    if ( Key::eB )
    {
      addBox( &renderer );
    }

    if ( Key::eL )
    {
      addSphere( &renderer );
    }
  }

  return 0;
}
