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
  renderer.scene( ).setCamera( std::make_shared<CustomCamera>( width, height, glm::vec3( 0.0F, 0.0F, 3.0F ) ) );

  // Custom window
  renderer.setWindow( std::make_shared<CustomWindow>( width, height, "Rayex Example", SDL_WINDOW_RESIZABLE, &renderer.scene( ) ) );

  // Custom ImGui based Gui
  renderer.setGui( std::make_shared<CustomGui>( &renderer ) );

  // Avoid buffer recreations by setting an upper limit to geometries and geometry instances.
  renderer.settings( ).setMaxGeometryInstances( 1000 );
  renderer.settings( ).setMaxGeoemtry( 8 );

  // ... and initialize the renderer.
  renderer.init( );

  loadCornellScene( &renderer );

  while ( renderer.isRunning( ) )
  {
    renderer.run( );

    /*
    if ( awpInstance1 != nullptr )
    {
      awpInstance1->setTransform( glm::rotate( awpInstance1->transform, rx::Time::getDeltaTime( ) * 0.5F, glm::vec3( 0.0F, 1.0F, 0.0F ) ) );
    }
    */

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
