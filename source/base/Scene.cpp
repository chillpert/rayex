#include "base/Scene.hpp"

#include "api/Components.hpp"

namespace RAYEX_NAMESPACE
{
  auto Scene::getGeometryInstances( ) const -> const std::vector<std::shared_ptr<GeometryInstance>>&
  {
    return this->geometryInstances;
  }

  void Scene::submitGeometryInstance( std::shared_ptr<GeometryInstance> geometryInstance )
  {
    uint32_t limit = this->settings->maxGeometryInstances.has_value( ) ? this->settings->maxGeometryInstances.value( ) : g_maxGeometryInstances;
    if ( this->geometryInstances.size( ) >= limit )
    {
      RX_ERROR( "Failed to submit geometry instance because instance buffer size has been exceeded. To avoid this error, increase the amount of supported geometry instances using RAYEX_NAMESPACE::Rayex::Settings::setMaxGeometryInstances(uint32_t)." );
      return;
    }

    this->geometryInstances.push_back( geometryInstance );
    this->uploadGeometryInstancesToBuffer = true;
  }

  void Scene::setGeometryInstances( const std::vector<std::shared_ptr<GeometryInstance>>& geometryInstances )
  {
    uint32_t limit = this->settings->maxGeometryInstances.has_value( ) ? this->settings->maxGeometryInstances.value( ) : g_maxGeometryInstances;
    if ( this->geometryInstances.size( ) >= limit )
    {
      RX_ERROR( "Failed to set geometry instances because instance buffer size has been exceeded. To avoid this error, increase the amount of supported geometry instances using RAYEX_NAMESPACE::Rayex::Settings::setMaxGeometryInstances(uint32_t)." );
      return;
    }

    this->geometryInstances               = geometryInstances;
    this->uploadGeometryInstancesToBuffer = true;
  }

  auto Scene::getDirectionalLights( ) const -> const std::vector<std::shared_ptr<DirectionalLight>>&
  {
    return this->directionalLights;
  }

  void Scene::submitDirectionalLight( std::shared_ptr<DirectionalLight> light )
  {
    uint32_t limit = this->settings->maxDirectionalLights.has_value( ) ? this->settings->maxDirectionalLights.value( ) : g_maxDirectionalLights;
    if ( this->directionalLights.size( ) >= limit )
    {
      RX_ERROR( "Failed to submit directional light because buffer size has been exceeded. To avoid this error, increase the amount of supported directional lights using RAYEX_NAMESPACE::Rayex::Settings::setMaxDirectionalLights(uint32_t)." );
      return;
    }

    this->directionalLights.push_back( light );
    this->uploadDirectionalLightsToBuffer = true;
  }

  void Scene::removeDirectionalLight( std::shared_ptr<DirectionalLight> light )
  {
    if ( light == nullptr )
    {
      RX_ERROR( "An invalid directional light can not be removed." );
      return;
    }

    std::vector<std::shared_ptr<DirectionalLight>> temp( this->directionalLights );
    this->directionalLights.clear( );
    this->directionalLights.reserve( temp.size( ) );

    for ( auto it : temp )
    {
      if ( it != light )
      {
        this->directionalLights.push_back( it );
      }
    }

    this->uploadDirectionalLightsToBuffer = true;
  }

  auto Scene::getPointLights( ) const -> const std::vector<std::shared_ptr<PointLight>>&
  {
    return this->pointLights;
  }

  void Scene::submitPointLight( std::shared_ptr<PointLight> light )
  {
    uint32_t limit = this->settings->maxPointLights.has_value( ) ? this->settings->maxPointLights.value( ) : g_maxPointLights;
    if ( this->pointLights.size( ) >= limit )
    {
      RX_ERROR( "Failed to submit point light because buffer size has been exceeded. To avoid this error, increase the amount of supported point lights using RAYEX_NAMESPACE::Rayex::Settings::setMaxPointLights(uint32_t)." );
      return;
    }

    this->pointLights.push_back( light );
    this->uploadPointLightsToBuffer = true;
  }

  void Scene::removePointLight( std::shared_ptr<PointLight> light )
  {
    if ( light == nullptr )
    {
      RX_ERROR( "An invalid point light can not be removed." );
      return;
    }

    std::vector<std::shared_ptr<PointLight>> temp( this->pointLights );
    this->pointLights.clear( );
    this->pointLights.reserve( temp.size( ) );

    for ( auto it : temp )
    {
      if ( it != light )
      {
        this->pointLights.push_back( it );
      }
    }

    this->uploadPointLightsToBuffer = true;
  }

  void Scene::removeGeometryInstance( std::shared_ptr<GeometryInstance> geometryInstance )
  {
    if ( geometryInstance == nullptr )
    {
      RX_ERROR( "An invalid geometry instance can not be removed." );
      return;
    }

    std::vector<std::shared_ptr<GeometryInstance>> temp( this->geometryInstances );
    this->geometryInstances.clear( );
    this->geometryInstances.reserve( temp.size( ) );

    for ( auto it : temp )
    {
      if ( it != geometryInstance )
      {
        this->geometryInstances.push_back( it );
      }
    }

    this->uploadGeometryInstancesToBuffer = true;
  }

  void Scene::submitGeometry( std::shared_ptr<Geometry> geometry )
  {
    if ( this->geometries.size( ) >= g_maxGeometryInstances - 1 )
    {
      RX_ERROR( "Failed to add geometry. You have exceeded the maximum amount of geometry supported." );
      return;
    }

    this->geometries.push_back( geometry );
    this->uploadGeometries = true;
  }

  void Scene::setGeometries( const std::vector<std::shared_ptr<Geometry>>& geometries )
  {
    if ( this->geometries.size( ) >= g_maxGeometryInstances )
    {
      RX_ERROR( "Failed to set geometries. You have exceeded the maximum amount of geometry supported." );
      return;
    }

    this->geometries       = geometries;
    this->uploadGeometries = true;
  }

  auto Scene::findGeometry( std::string_view path ) const -> std::shared_ptr<Geometry>
  {
    for ( std::shared_ptr<Geometry> geometry : this->geometries )
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
