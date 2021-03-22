#include "CustomCamera.hpp"
#include "CustomGui.hpp"
#include "CustomWindow.hpp"

int main( )
{
  // Window dimensions.
  const int width  = 1920;
  const int height = 1080;

  rx::Rayex renderer;

  // Custom camera
  renderer.scene( ).setCamera( std::make_shared<CustomCamera>( width, height, glm::vec3( 0.0F, 0.0F, 3.0F ) ) );

  // Custom window
  renderer.setWindow( std::make_shared<CustomWindow>( width, height, "Rayex Example", SDL_WINDOW_RESIZABLE, &renderer.scene( ) ) );

  // Custom ImGui based Gui
  renderer.setGui( std::make_shared<CustomGui>( &renderer ) );

  // ... and initialize the renderer.
  renderer.init( );

  loadScene( &renderer, Level::eMirrors );

  while ( renderer.isRunning( ) )
  {
    updateScene( &renderer );

    renderer.run( );
  }

  return 0;
}
