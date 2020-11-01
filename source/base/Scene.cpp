#include "base/Scene.hpp"

#include "api/Components.hpp"

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

  auto Scene::getDirectionalLights( ) const -> const std::vector<std::shared_ptr<DirectionalLight>>&
  {
    return _directionalLights;
  }

  void Scene::submitDirectionalLight( std::shared_ptr<DirectionalLight> light )
  {
    if ( _directionalLights.size( ) >= _settings->_maxDirectionalLights )
    {
      RX_ERROR( "Failed to submit directional light because buffer size has been exceeded. To avoid this error, increase the amount of supported directional lights using RAYEX_NAMESPACE::Rayex::Settings::setMaxDirectionalLights(uint32_t)." );
      return;
    }

    _directionalLights.push_back( light );
    _uploadDirectionalLightsToBuffer = true;
  }

  void Scene::removeDirectionalLight( std::shared_ptr<DirectionalLight> light )
  {
    if ( light == nullptr )
    {
      RX_ERROR( "An invalid directional light can not be removed." );
      return;
    }

    std::vector<std::shared_ptr<DirectionalLight>> temp( _directionalLights );
    _directionalLights.clear( );
    _directionalLights.reserve( temp.size( ) );

    for ( auto it : temp )
    {
      if ( it != light )
      {
        _directionalLights.push_back( it );
      }
    }

    _uploadDirectionalLightsToBuffer = true;
  }

  auto Scene::getPointLights( ) const -> const std::vector<std::shared_ptr<PointLight>>&
  {
    return _pointLights;
  }

  void Scene::submitPointLight( std::shared_ptr<PointLight> light )
  {
    if ( _pointLights.size( ) >= _settings->_maxPointLights )
    {
      RX_ERROR( "Failed to submit point light because buffer size has been exceeded. To avoid this error, increase the amount of supported point lights using RAYEX_NAMESPACE::Rayex::Settings::setMaxPointLights(uint32_t)." );
      return;
    }

    _pointLights.push_back( light );
    _uploadPointLightsToBuffer = true;
  }

  void Scene::removePointLight( std::shared_ptr<PointLight> light )
  {
    if ( light == nullptr )
    {
      RX_ERROR( "An invalid point light can not be removed." );
      return;
    }

    std::vector<std::shared_ptr<PointLight>> temp( _pointLights );
    _pointLights.clear( );
    _pointLights.reserve( temp.size( ) );

    for ( auto it : temp )
    {
      if ( it != light )
      {
        _pointLights.push_back( it );
      }
    }

    _uploadPointLightsToBuffer = true;
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

    // @todo Remove textures of each material
    for ( auto it : _geometries )
    {
      bool found                 = false;
      std::string diffuseTexPath = "";

      for ( const auto& mesh : it->meshes )
      {
        diffuseTexPath = mesh.material.diffuseTexPath;

        for ( const auto& thisMesh : geometry->meshes )
        {
          if ( !mesh.material.diffuseTexPath.empty( ) && !thisMesh.material.diffuseTexPath.empty( ) )
          {
            if ( mesh.material.diffuseTexPath == thisMesh.material.diffuseTexPath )
            {
              diffuseTexPath = mesh.material.diffuseTexPath;
              found          = true;
              break;
            }
          }
        }
      }

      if ( found )
      {
        RX_ERROR( diffuseTexPath, " still needed." );
      }
    }
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
    RX_ASSERT( false, "NOT IMPLEMENTED" );
    // Remove all instances.
    for ( auto geometry : _geometries )
    {
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
    }

    _geometries.clear( );

    // Reset index counter.
    components::geometryIndex = 0;

    // Reset texture counter.
    components::textureIndex = 0;

    _uploadGeometries = true;
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
} // namespace RAYEX_NAMESPACE
