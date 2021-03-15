#include "Rayex.hpp"

enum class Level
{
  eCornell,
  eAnimations,
  eSpheres,
  eMirrors,
  eStressTest
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
    renderer->settings( ).setGeometryLimit( 1000 ); // Will give a warning.
    renderer->settings( ).setGeometryInstanceLimit( 10000 );
    renderer->settings( ).setTextureLimit( 1000 ); // Will give a warning.
    renderer->settings( ).setMeshLimit( 1000 );
    renderer->settings( ).setClearColor( glm::vec4( 0.45F, 0.45F, 0.45F, 0.8F ) );
    renderer->settings( ).setAccumulatingFrames( true );

    auto cornell = rx::loadObj( "models/CornellBox.obj" );
    // Set lamp submesh emittance.
    cornell->meshes[7].material.emission = glm::vec3( 10.0F );

    renderer->scene( ).setGeometries( { cornell } );

    auto transform       = glm::translate( glm::mat4( 1.0F ), glm::vec3( 0.0F, -1.0F, -1.0F ) );
    auto cornellInstance = rx::instance( cornell, transform );

    renderer->scene( ).setGeometryInstances( { cornellInstance } );

    renderer->scene( ).removeEnvironmentMap( );
  }
  else if ( scene == Level::eAnimations )
  {
    renderer->reset( );
    renderer->settings( ).setGeometryLimit( 5 ); // Will give a warning.
    renderer->settings( ).setGeometryInstanceLimit( 4000 );
    renderer->settings( ).setTextureLimit( 4 ); // Will give a warning.

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
    renderer->settings( ).setGeometryLimit( 100 ); // Will give a warning.
    renderer->settings( ).setGeometryInstanceLimit( 15000 );
    renderer->settings( ).setTextureLimit( 100 ); // Will give a warning.
    renderer->settings( ).setMeshLimit( 100 );

    renderer->settings( ).setAccumulatingFrames( false );

    renderer->settings( ).setClearColor( glm::vec4( 0.0F, 0.0F, 0.0F, 1.0F ) );

    auto lightPlane = rx::loadObj( "models/plane.obj" );
    rx::Material lightMaterial;
    lightMaterial.emission = glm::vec3( 1.0F );
    lightPlane->setMaterial( lightMaterial );

    auto floor = rx::loadObj( "models/plane.obj" );
    rx::Material floorMaterial;
    floorMaterial.kd = glm::vec3( 1.0F );

    floor->setMaterial( floorMaterial );

    auto sphere = rx::loadObj( "models/sphere.obj" );
    rx::Material mat;
    mat.kd    = glm::vec3( 1.0F );
    mat.ni    = 1.0F;
    mat.illum = 1;
    mat.ni    = 2.4F;
    sphere->setMaterial( mat );

    auto sphere1 = rx::loadObj( "models/sphere.obj" );
    mat.kd       = glm::vec3( 1.0F, 0.8F, 0.0F );
    mat.ni       = 1;
    mat.illum    = 0;
    mat.d        = 0.5;
    mat.ni       = 1.0F;
    sphere1->setMaterial( mat );

    auto sphere2 = rx::loadObj( "models/sphere.obj" );
    mat.illum    = 0;
    mat.ni       = 0.0F;
    mat.d        = 1.0;
    sphere2->setMaterial( mat );

    auto sphere3 = rx::loadObj( "models/sphere.obj" );
    mat.illum    = 2;
    mat.kd       = glm::vec3( 1.0F, 0.8F, 0.0F );
    sphere3->setMaterial( mat );

    auto sphere4  = rx::loadObj( "models/sphere.obj" );
    mat.illum     = 2;
    mat.fuzziness = 0.2F;
    sphere4->setMaterial( mat );

    auto sphere5  = rx::loadObj( "models/sphere.obj" );
    mat.illum     = 2;
    mat.fuzziness = 0.6F;
    sphere5->setMaterial( mat );

    auto sphere6  = rx::loadObj( "models/sphere.obj" );
    mat.illum     = 2;
    mat.fuzziness = 0.8F;
    sphere6->setMaterial( mat );

    auto sphere7  = rx::loadObj( "models/sphere.obj" );
    mat.illum     = 2;
    mat.fuzziness = 1.0F;
    sphere7->setMaterial( mat );

    auto glass = rx::loadObj( "models/plane.obj" );
    rx::Material glassMaterial;
    glassMaterial.kd    = glm::vec3( 1.0F );
    glassMaterial.ni    = 1.0F;
    glassMaterial.illum = 1;
    glassMaterial.ni    = 1.6;
    glass->setMaterial( glassMaterial );

    renderer->scene( ).setGeometries( { lightPlane, floor, sphere, sphere1, sphere2, sphere3, sphere4, sphere5, sphere6, sphere7, glass } );

    auto transform          = glm::translate( glm::mat4( 1.0F ), glm::vec3( 0.0F, 80.0F, 0.0F ) );
    auto lightPlaneInstance = rx::instance( lightPlane, transform );

    transform          = glm::scale( glm::mat4( 1.0F ), glm::vec3( 0.2F, 0.2F, 0.2F ) );
    auto floorInstance = rx::instance( floor, transform );

    transform           = glm::translate( glm::mat4( 1.0F ), glm::vec3( -5.0F, 0.0F, 0.0F ) );
    auto sphereInstance = rx::instance( sphere, transform );

    transform            = glm::translate( glm::mat4( 1.0F ), glm::vec3( -2.5F, 0.0F, 0.0F ) );
    auto sphereInstance1 = rx::instance( sphere1, transform );

    transform            = glm::translate( glm::mat4( 1.0F ), glm::vec3( 0.0F, 0.0F, 0.0F ) );
    auto sphereInstance2 = rx::instance( sphere2, transform );

    transform            = glm::translate( glm::mat4( 1.0F ), glm::vec3( 2.5F, 0.0F, 0.0F ) );
    auto sphereInstance3 = rx::instance( sphere3, transform );

    transform            = glm::translate( glm::mat4( 1.0F ), glm::vec3( 5.0F, 0.0F, 0.0F ) );
    auto sphereInstance4 = rx::instance( sphere4, transform );

    transform            = glm::translate( glm::mat4( 1.0F ), glm::vec3( 7.5F, 0.0F, 0.0F ) );
    auto sphereInstance5 = rx::instance( sphere5, transform );

    transform            = glm::translate( glm::mat4( 1.0F ), glm::vec3( 10.0F, 0.0F, 0.0F ) );
    auto sphereInstance6 = rx::instance( sphere6, transform );

    transform            = glm::translate( glm::mat4( 1.0F ), glm::vec3( 12.5F, 0.0F, 0.0F ) );
    auto sphereInstance7 = rx::instance( sphere7, transform );

    transform          = glm::scale( glm::mat4( 1.0F ), glm::vec3( 0.05F, 0.05F, 0.05F ) );
    transform          = glm::rotate( transform, glm::radians( 90.0F ), glm::vec3( 1.0F, 0.0F, 0.0F ) );
    transform          = glm::translate( transform, glm::vec3( 0.0F, 250.0F, 0.0F ) );
    auto glassInstance = rx::instance( glass, transform );

    renderer->scene( ).setGeometryInstances( { lightPlaneInstance, floorInstance, sphereInstance, sphereInstance1, sphereInstance2, sphereInstance3, sphereInstance4, sphereInstance5, sphereInstance6, sphereInstance7, glassInstance } );
  }
  else if ( scene == Level::eMirrors )
  {
    renderer->reset( );
    renderer->settings( ).setGeometryLimit( 3 ); // Will give a warning.
    renderer->settings( ).setGeometryInstanceLimit( 1000 );
    renderer->settings( ).setTextureLimit( 2 ); // Will give a warning.

    renderer->settings( ).setAccumulatingFrames( true );
    renderer->settings( ).setClearColor( glm::vec4( 0.5F, 0.5F, 0.7F, 1.0F ) );

    renderer->scene( ).removeEnvironmentMap( );

    auto lightPlane = rx::loadObj( "models/plane.obj" );
    rx::Material lightMaterial;
    lightMaterial.emission = glm::vec3( 1.0F );
    lightPlane->setMaterial( lightMaterial );

    auto mirrorPlane = rx::loadObj( "models/plane.obj" );
    rx::Material mirrorMaterial;
    mirrorMaterial.illum     = 2;
    mirrorMaterial.fuzziness = 0.0F;
    mirrorMaterial.kd        = glm::vec3( 1.0F );
    mirrorPlane->setMaterial( mirrorMaterial );

    renderer->scene( ).setGeometries( { lightPlane, mirrorPlane } );

    auto transform          = glm::translate( glm::mat4( 1.0F ), glm::vec3( 0.0F, 80.0F, 0.0F ) );
    auto lightPlaneInstance = rx::instance( lightPlane, transform );

    transform                 = glm::scale( glm::mat4( 1.0F ), glm::vec3( 0.1F ) );
    transform                 = glm::rotate( transform, glm::radians( 180.0F ), glm::vec3( 0.0F, 1.0F, 0.0F ) );
    transform                 = glm::rotate( transform, glm::radians( 90.0F ), glm::vec3( 1.0F, 0.0F, 0.0F ) );
    transform                 = glm::translate( transform, glm::vec3( 0.0F, -100.0F, 0.0F ) );
    auto mirrorPlaneInstance1 = rx::instance( mirrorPlane, transform );

    transform                 = glm::scale( glm::mat4( 1.0F ), glm::vec3( 0.1F ) );
    transform                 = glm::rotate( transform, glm::radians( 90.0F ), glm::vec3( 1.0F, 0.0F, 0.0F ) );
    transform                 = glm::translate( transform, glm::vec3( 0.0F, -100.0F, 0.0F ) );
    auto mirrorPlaneInstance2 = rx::instance( mirrorPlane, transform );

    renderer->scene( ).setGeometryInstances( { mirrorPlaneInstance1, mirrorPlaneInstance2, lightPlaneInstance } );

    for ( int i = 1; i < 1000; ++i )
    {
      addSphere( renderer );
    }
  }
  else if ( scene == Level::eStressTest )
  {
    renderer->reset( );
    renderer->settings( ).setGeometryLimit( 3 ); // Will give a warning.
    renderer->settings( ).setGeometryInstanceLimit( 15000 );
    renderer->settings( ).setTextureLimit( 2 ); // Will give a warning.

    renderer->settings( ).setAccumulatingFrames( true );
    renderer->settings( ).setClearColor( glm::vec4( 0.5F, 0.5F, 0.7F, 1.0F ) );

    renderer->scene( ).removeEnvironmentMap( );

    auto lightPlane = rx::loadObj( "models/plane.obj" );
    rx::Material lightMaterial;
    lightMaterial.emission = glm::vec3( 1.0F );
    lightPlane->setMaterial( lightMaterial );

    renderer->scene( ).setGeometries( { lightPlane } );

    auto transform          = glm::translate( glm::mat4( 1.0F ), glm::vec3( 0.0F, 80.0F, 0.0F ) );
    auto lightPlaneInstance = rx::instance( lightPlane, transform );

    renderer->scene( ).setGeometryInstances( { lightPlaneInstance } );

    for ( int i = 1; i < 7500; ++i )
    {
      addSphere( renderer );
    }
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

  if ( currentLevel == Level::eAnimations )
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
}
