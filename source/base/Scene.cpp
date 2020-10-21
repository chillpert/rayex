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
    uint32_t limit = _settings->_maxGeometryInstances.has_value( ) ? _settings->_maxGeometryInstances.value( ) : g_maxGeometryInstances;
    if ( _geometryInstances.size( ) >= limit )
    {
      RX_ERROR( "Failed to submit geometry instance because instance buffer size has been exceeded. To avoid this error, increase the amount of supported geometry instances using RAYEX_NAMESPACE::Rayex::Settings::setMaxGeometryInstances(uint32_t)." );
      return;
    }

    _geometryInstances.push_back( geometryInstance );
    _uploadGeometryInstancesToBuffer = true;
  }

  void Scene::setGeometryInstances( const std::vector<std::shared_ptr<GeometryInstance>>& geometryInstances )
  {
    uint32_t limit = _settings->_maxGeometryInstances.has_value( ) ? _settings->_maxGeometryInstances.value( ) : g_maxGeometryInstances;
    if ( _geometryInstances.size( ) >= limit )
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
    uint32_t limit = _settings->_maxDirectionalLights.has_value( ) ? _settings->_maxDirectionalLights.value( ) : g_maxDirectionalLights;
    if ( _directionalLights.size( ) >= limit )
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
    uint32_t limit = _settings->_maxPointLights.has_value( ) ? _settings->_maxPointLights.value( ) : g_maxPointLights;
    if ( _pointLights.size( ) >= limit )
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
    if ( _geometries.size( ) >= g_maxGeometryInstances - 1 )
    {
      RX_ERROR( "Failed to add geometry. You have exceeded the maximum amount of geometry supported." );
      return;
    }

    _geometries.push_back( geometry );
    _uploadGeometries = true;
  }

  void Scene::setGeometries( const std::vector<std::shared_ptr<Geometry>>& geometries )
  {
    if ( _geometries.size( ) >= g_maxGeometryInstances )
    {
      RX_ERROR( "Failed to set geometries. You have exceeded the maximum amount of geometry supported." );
      return;
    }

    _geometries       = geometries;
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

    RX_INFO( "Could not find geometry in scene. Trying to create geometry instead." );
    return loadObj( path );
  }
} // namespace RAYEX_NAMESPACE
