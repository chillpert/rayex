#include "Rayex.hpp"

enum class Level
{
  eCornell,
  eDebug,
  eSpheres
};

inline Level currentLevel;

inline auto getRandomUniquePosition( float min, float max ) -> glm::vec3
{
  static std::vector<glm::vec3> positions;

  static std::random_device rd;
  static std::mt19937 mt( rd( ) );
  std::uniform_real_distribution<float> dist( min, max );

  glm::vec3 result = glm::vec3( 0.0F );

  while ( true )
  {
    result.x = dist( mt );
    result.y = dist( mt );
    result.z = dist( mt );

    bool accepted = true;
    for ( const auto& position : positions )
    {
      if ( result == position )
      {
        accepted = false;
      }
    }

    if ( accepted )
    {
      break;
    }
  };

  return result;
}

inline auto getRandomFloat( float min, float max ) -> float
{
  static std::random_device rd;
  static std::mt19937 mt( rd( ) );
  std::uniform_real_distribution<float> dist( min, max );

  return dist( mt );
}

inline void addBox( rx::Rayex* renderer )
{
  std::string_view path = "models/cube.obj";
  auto cube             = renderer->scene( ).findGeometry( path );
  if ( cube == nullptr )
  {
    cube = rx::loadObj( path );
    renderer->scene( ).submitGeometry( cube );
  }

  auto transform = glm::scale( glm::mat4( 1.0F ), glm::vec3( 0.3F, 0.3F, 0.3F ) );
  transform      = glm::rotate( transform, getRandomFloat( 0.0F, 360.0F ), glm::vec3( 0.0F, 1.0F, 0.0F ) );
  transform      = glm::translate( transform, getRandomUniquePosition( -25.0F, 25.0F ) );

  auto cubeInstance = rx::instance( cube, transform );
  renderer->scene( ).submitGeometryInstance( cubeInstance );
}

inline void addSphere( rx::Rayex* renderer )
{
  std::string_view path = "models/sphere.obj";
  auto sphere           = renderer->scene( ).findGeometry( path );
  if ( sphere == nullptr )
  {
    sphere = rx::loadObj( path );
    renderer->scene( ).submitGeometry( sphere );
  }

  auto transform = glm::scale( glm::mat4( 1.0F ), glm::vec3( 0.1F, 0.1F, 0.1F ) );
  transform      = glm::rotate( transform, getRandomFloat( 0.0F, 360.0F ), glm::vec3( 0.0F, 1.0F, 0.0F ) );
  transform      = glm::translate( transform, getRandomUniquePosition( -70.0F, 70.0F ) );

  auto sphereInstance = rx::instance( sphere, transform );
  renderer->scene( ).submitGeometryInstance( sphereInstance );
}

inline void addAwp( rx::Rayex* renderer )
{
  std::string_view path = "models/awpdlore/awpdlore.obj";
  auto awp              = renderer->scene( ).findGeometry( path );
  if ( awp == nullptr )
  {
    awp = rx::loadObj( path );
    renderer->scene( ).submitGeometry( awp );
  }

  auto transform = glm::scale( glm::mat4( 1.0F ), glm::vec3( 0.3F, 0.3F, 0.3F ) );
  transform      = glm::rotate( transform, getRandomFloat( 0.0F, 360.0F ), glm::vec3( 0.0F, 1.0F, 0.0F ) );
  transform      = glm::translate( transform, getRandomUniquePosition( -10.0F, 10.0F ) );

  auto awpInstance = rx::instance( awp, transform );
  renderer->scene( ).submitGeometryInstance( awpInstance );
}

inline void loadScene( rx::Rayex* renderer, Level scene )
{
  currentLevel = scene;

  renderer->scene( ).getCamera( )->reset( );

  if ( scene == Level::eCornell )
  {
    renderer->reset( );
    renderer->settings( ).setMaxGeoemtry( 1 ); // Will give a warning.
    renderer->settings( ).setMaxGeometryInstances( 1 );
    renderer->settings( ).setMaxTextures( 0 ); // Will give a warning.

    // Enable frame accumulation because scene is static.
    renderer->settings( ).setAccumulatingFrames( true );

    auto cornell = rx::loadObj( "models/CornellBox.obj" );
    // Set lamp submesh emittance.
    cornell->meshes[7].material.emission = glm::vec3( 10.0F );

    renderer->scene( ).setGeometries( { cornell } );

    auto transform       = glm::translate( glm::mat4( 1.0F ), glm::vec3( 0.75F, -1.0F, -1.0F ) );
    auto cornellInstance = rx::instance( cornell, transform );

    renderer->scene( ).setGeometryInstances( { cornellInstance } );

    renderer->scene( ).removeEnvironmentMap( );
  }
  else if ( scene == Level::eDebug )
  {
    renderer->reset( );
    renderer->settings( ).setMaxGeoemtry( 5 ); // Will give a warning.
    renderer->settings( ).setMaxGeometryInstances( 4000 );
    renderer->settings( ).setMaxTextures( 4 ); // Will give a warning.

    // Disable frame accumulation because scene contains moving elements.
    renderer->settings( ).setAccumulatingFrames( false );

    // Load geometries.
    auto awp   = rx::loadObj( "models/awpdlore/awpdlore.obj" );
    auto plane = rx::loadObj( "models/plane.obj" );

    // Make a custom material for an emissive surface (light source).
    rx::Material customMaterial;
    customMaterial.emission = glm::vec3( 1.0F );
    plane->setMaterial( customMaterial );

    // Submit geometries.
    renderer->scene( ).setGeometries( { awp, plane } );

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

    // Submit instances for drawing.
    renderer->scene( ).setGeometryInstances( { awpInstance1, awpInstance2, planeInstance } );

    renderer->scene( ).setEnvironmentMap( "models/skybox/cubemap_yokohama_rgba.ktx" );
  }
  else if ( scene == Level::eSpheres )
  {
    renderer->reset( );
    renderer->settings( ).setMaxGeoemtry( 1 ); // Will give a warning.
    renderer->settings( ).setMaxGeometryInstances( 1000 );
    renderer->settings( ).setMaxTextures( 1 ); // Will give a warning.

    renderer->settings( ).setAccumulatingFrames( true );

    auto sphere = rx::loadObj( "models/sphere.obj" );
    rx::Material mat;
    mat.illum = 2;
    mat.ns    = 1000.0F;
    mat.kd    = glm::vec3( 0.0F, 0.0F, 1.0F );
    mat.ks    = glm::vec3( 1.0F, 1.0F, 1.0F );
    sphere->setMaterial( mat );

    auto plane = rx::loadObj( "models/plane.obj" );

    // Make a custom material for an emissive surface (light source).
    rx::Material mat2;
    mat2.emission = glm::vec3( 1.0F );
    plane->setMaterial( mat2 );

    renderer->scene( ).setGeometries( { sphere, plane } );

    auto sphereInstance = rx::instance( sphere );

    auto transform = glm::scale( glm::mat4( 1.0F ), glm::vec3( 1.0F ) );
    transform      = glm::translate( transform, glm::vec3( 0.0F, 80.0F, 0.0F ) );

    auto planeInstance = rx::instance( plane, transform );

    renderer->scene( ).setGeometryInstances( { sphereInstance, planeInstance } );

    renderer->scene( ).setEnvironmentMap( "models/skybox/cubemap_yokohama_rgba.ktx" );
  }
}

void updateScene( rx::Rayex* renderer )
{
  if ( Key::eB )
  {
    addBox( renderer );
  }

  if ( Key::eL )
  {
    addSphere( renderer );
  }

  if ( currentLevel == Level::eCornell )
  {
    // No updates needed.
  }
  else if ( currentLevel == Level::eDebug )
  {
    auto instances = renderer->scene( ).getGeometryInstances( );

    if ( !instances.empty( ) )
    {
      if ( instances[0] != nullptr )
      {
        instances[0]->setTransform( glm::rotate( instances[0]->transform, rx::Time::getDeltaTime( ) * 0.5F, glm::vec3( 0.0F, 1.0F, 0.0F ) ) );
      }
    }
  }
  else if ( currentLevel == Level::eSpheres )
  {
  }
}
