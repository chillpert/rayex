#include "base/Scene.hpp"

#include "api/Components.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "external/gltf/tiny_gltf.h"

namespace RAYEX_NAMESPACE
{
  auto Scene::getGeometryInstances( ) const -> const std::vector<std::shared_ptr<GeometryInstance>>&
  {
    return _geometryInstances;
  }

  void Scene::submitGeometryInstance( std::shared_ptr<GeometryInstance> geometryInstance )
  {
    if ( _geometryInstances.size( ) >= _settings->_maxGeometryInstances - 1 )
    {
      RX_ERROR( "Failed to submit geometry instance because instance buffer size has been exceeded. To avoid this error, increase the amount of supported geometry instances using RAYEX_NAMESPACE::Rayex::Settings::setMaxGeometryInstances(uint32_t)." );
      return;
    }

    _geometryInstances.push_back( geometryInstance );
    _uploadGeometryInstancesToBuffer = true;
  }

  void Scene::setGeometryInstances( const std::vector<std::shared_ptr<GeometryInstance>>& geometryInstances )
  {
    if ( _geometryInstances.size( ) >= _settings->_maxGeometryInstances - 1 )
    {
      RX_ERROR( "Failed to set geometry instances because instance buffer size has been exceeded. To avoid this error, increase the amount of supported geometry instances using RAYEX_NAMESPACE::Rayex::Settings::setMaxGeometryInstances(uint32_t)." );
      return;
    }

    _geometryInstances               = geometryInstances;
    _uploadGeometryInstancesToBuffer = true;
  }

  void Scene::removeGeometryInstance( std::shared_ptr<GeometryInstance> geometryInstance )
  {
    if ( geometryInstance == nullptr )
    {
      RX_ERROR( "An invalid geometry instance can not be removed." );
      return;
    }

    std::vector<std::shared_ptr<GeometryInstance>> temp( _geometryInstances );
    _geometryInstances.clear( );
    _geometryInstances.reserve( temp.size( ) );

    for ( auto it : temp )
    {
      if ( it != geometryInstance )
      {
        _geometryInstances.push_back( it );
      }
    }

    _uploadGeometryInstancesToBuffer = true;
  }

  void Scene::clearGeometryInstances( )
  {
    // Only allow clearing the scene if there is no dummy element.
    if ( !_dummy )
    {
      _geometryInstances.clear( );
      _uploadGeometryInstancesToBuffer = true;
    }
  }

  void Scene::popGeometryInstance( )
  {
    // Only allow clearing the scene if the scene is not empty and does not contain a dummy element.
    if ( !_geometryInstances.empty( ) && !_dummy )
    {
      _geometryInstances.erase( _geometryInstances.end( ) - 1 );
      _uploadGeometryInstancesToBuffer = true;
    }
  }

  void Scene::submitGeometry( std::shared_ptr<Geometry> geometry )
  {
    if ( _geometries.size( ) >= _settings->_maxGeometryInstances - 1 )
    {
      RX_ERROR( "Failed to add geometry. You have exceeded the maximum amount of geometry supported." );
      return;
    }

    _geometries.push_back( geometry );
    _uploadGeometries = true;
  }

  void Scene::setGeometries( const std::vector<std::shared_ptr<Geometry>>& geometries )
  {
    if ( _geometries.size( ) >= _settings->_maxGeometryInstances )
    {
      RX_ERROR( "Failed to set geometries. You have exceeded the maximum amount of geometry supported." );
      return;
    }

    _geometries       = geometries;
    _uploadGeometries = true;
  }

  void Scene::removeGeometry( std::shared_ptr<Geometry> geometry )
  {
    if ( geometry == nullptr )
    {
      RX_ERROR( "An invalid geometry can not be removed." );
      return;
    }

    // Removing a geometry also means removing all its instances.
    std::vector<std::shared_ptr<GeometryInstance>> instancesToDelete;
    for ( auto it : _geometryInstances )
    {
      if ( it->geometryIndex == geometry->geometryIndex )
      {
        instancesToDelete.push_back( it );
      }
    }

    for ( auto it : instancesToDelete )
    {
      removeGeometryInstance( it );
    }

    std::vector<std::shared_ptr<Geometry>> temp( _geometries );
    _geometries.clear( );
    _geometries.reserve( temp.size( ) );

    uint32_t geometryIndex = 0;
    for ( auto it : temp )
    {
      if ( it != geometry )
      {
        it->geometryIndex = geometryIndex++;
        _geometries.push_back( it );
      }
    }

    --components::geometryIndex;
    _uploadGeometries = true; // @todo Might not be necessary.

    // Update geometry indices for geometry instances.
    std::vector<std::shared_ptr<GeometryInstance>> temp2( _geometryInstances );
    _geometryInstances.clear( );
    _geometries.reserve( temp2.size( ) );

    geometryIndex = 0;
    for ( auto it : temp2 )
    {
      if ( it->geometryIndex > geometry->geometryIndex )
      {
        --it->geometryIndex;
        _geometryInstances.push_back( it );
      }
      else
      {
        _geometryInstances.push_back( it );
      }
    }

    _uploadGeometryInstancesToBuffer = true;
  }

  void Scene::removeGeometry( uint32_t geometryIndex )
  {
    for ( auto it : _geometries )
    {
      if ( it->geometryIndex == geometryIndex )
      {
        removeGeometry( it );
        break;
      }
    }
  }

  void Scene::clearGeometries( )
  {
    RX_INFO( "Clearing geoemtry." );

    _geometries.clear( );
    _geometryInstances.clear( );

    // Reset index counter.
    components::geometryIndex = 0;

    // Reset texture counter.
    components::textureIndex = 0;

    _deleteTextures                  = true;
    _uploadGeometries                = true;
    _uploadGeometryInstancesToBuffer = true;
  }

  void Scene::popGeometry( )
  {
    if ( !_geometries.empty( ) )
    {
      removeGeometry( *( _geometries.end( ) - 1 ) );
    }
  }

  auto Scene::findGeometry( std::string_view path ) const -> std::shared_ptr<Geometry>
  {
    for ( std::shared_ptr<Geometry> geometry : _geometries )
    {
      if ( geometry->path == path )
      {
        return geometry;
      }
    }

    RX_INFO( "Could not find geometry in scene." );
    return nullptr;
  }

  void Scene::setEnvironmentMap( std::string_view path )
  {
    _environmentMapTexturePath = path;
    _useEnvironmentMap         = true;
    _uploadEnvironmentMap      = true;
  }

  void Scene::removeEnvironmentMap( )
  {
    _useEnvironmentMap = false;
  }

  void Scene::setCamera( std::shared_ptr<Camera> camera )
  {
    _cameras.insert( camera );
    _currentCamera = camera;
  }

  void Scene::load( const std::string& path )
  {
    // This might need to be moved to api.cpp anyways, because it requires direct access to some of the buffers apparently.
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;

    std::string errors;
    std::string warnings;

    bool res = loader.LoadASCIIFromFile( &model, &errors, &warnings, path );

    if ( !warnings.empty( ) )
    {
      RX_WARN( "GLTF: ", warnings );
    }

    if ( !errors.empty( ) )
    {
      RX_ERROR( "GLTF: ", errors );
    }

    RX_ASSERT( res, "Failed to parse gltf file at ", path );

    RX_WARN( "GLTF support not implemented yet." );
  }

} // namespace RAYEX_NAMESPACE
