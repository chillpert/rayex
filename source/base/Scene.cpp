#include "base/Scene.hpp"

#include "api/Components.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "external/gltf/tiny_gltf.h"

namespace RAYEX_NAMESPACE
{
  CameraUbo cameraUbo;

  std::shared_ptr<Geometry> triangle                 = nullptr; ///< A dummy triangle that will be placed in the scene if it empty. This assures the AS creation.
  std::shared_ptr<GeometryInstance> triangleInstance = nullptr;

  std::vector<GeometryInstanceSSBO> memAlignedGeometryInstances;
  std::vector<MeshSSBO> memAlignedMeshes;

  auto Scene::getGeometryInstances( ) const -> const std::vector<std::shared_ptr<GeometryInstance>>&
  {
    return _geometryInstances;
  }

  void Scene::submitGeometryInstance( std::shared_ptr<GeometryInstance> geometryInstance )
  {
    if ( !_dummy )
    {
      if ( _geometryInstances.size( ) >= _settings->_maxGeometryInstances - 1 )
      {
        RX_ERROR( "Failed to submit geometry instance because instance buffer size has been exceeded. To avoid this error, increase the amount of supported geometry instances using RAYEX_NAMESPACE::Rayex::Settings::setMaxGeometryInstances(size_t)." );
        return;
      }
    }

    _geometryInstances.push_back( geometryInstance );
    _uploadGeometryInstancesToBuffer = true;
  }

  void Scene::setGeometryInstances( const std::vector<std::shared_ptr<GeometryInstance>>& geometryInstances )
  {
    _geometryInstances.clear( );
    _geometryInstances.reserve( geometryInstances.size( ) );

    for ( auto geometryInstance : geometryInstances )
    {
      submitGeometryInstance( geometryInstance );
    }

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
    if ( !_dummy )
    {
      if ( _geometries.size( ) >= _settings->_maxGeometryInstances - 1 )
      {
        RX_ERROR( "Failed to submit geometry because geometries buffer size has been exceeded. To avoid this error, increase the amount of supported geometries using RAYEX_NAMESPACE::Rayex::Settings::setMaxGeometries(size_t)." );
        return;
      }
    }

    _geometries.push_back( geometry );
    _uploadGeometries = true;
  }

  void Scene::setGeometries( const std::vector<std::shared_ptr<Geometry>>& geometries )
  {
    _geometries.clear( );
    _geometries.reserve( geometries.size( ) );

    for ( auto geometry : geometries )
    {
      submitGeometry( geometry );
    }

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
    RX_INFO( "Clearing geometry." );

    _geometries.clear( );
    _geometryInstances.clear( );

    // Reset index counter.
    components::geometryIndex = 0;

    // Reset texture counter.
    components::textureIndex = 0;

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

  void Scene::prepareBuffers( )
  {
    // Resize and initialize buffers with "dummy data".
    // The advantage of doing this is that the buffers are all initialized right away (even though it is invalid data) and
    // this makes it possible to call fill instead of initialize again, when changing any of the data below.
    std::vector<GeometryInstanceSSBO> geometryInstances( _settings->_maxGeometryInstances );
    _geometryInstancesBuffer.init( geometryInstances, components::maxResources );

    _vertexBuffers.resize( _settings->_maxGeometry );
    _indexBuffers.resize( _settings->_maxGeometry );
    _meshBuffers.resize( _settings->_maxMeshes );
    _textures.resize( _settings->_maxTextures );

    initCameraBuffer( );
  }

  void Scene::initCameraBuffer( )
  {
    _cameraUniformBuffer.init( );
  }

  void Scene::uploadCameraBuffer( uint32_t imageIndex )
  {
    // Upload camera.
    if ( _currentCamera != nullptr )
    {
      if ( _currentCamera->_updateView )
      {
        cameraUbo.view        = _currentCamera->getViewMatrix( );
        cameraUbo.viewInverse = _currentCamera->getViewInverseMatrix( );

        _currentCamera->_updateView = false;
      }

      if ( _currentCamera->_updateProj )
      {
        cameraUbo.projection        = _currentCamera->getProjectionMatrix( );
        cameraUbo.projectionInverse = _currentCamera->getProjectionInverseMatrix( );

        _currentCamera->_updateProj = false;
      }

      cameraUbo.position = glm::vec4( _currentCamera->getPosition( ), 1.0F );
    }

    _cameraUniformBuffer.upload( imageIndex, cameraUbo );
  }

  void Scene::uploadEnvironmentMap( )
  {
    _uploadEnvironmentMap = false;

    _environmentMap.init( _environmentMapTexturePath );

    if ( _removeEnvironmentMap )
    {
      removeEnvironmentMap( );
      _removeEnvironmentMap = false;
    }
  }

  void Scene::uploadGeometries( )
  {
    _uploadGeometries = false;

    for ( size_t i = 0; i < _geometries.size( ); ++i )
    {
      if ( i < _geometries.size( ) )
      {
        if ( _geometries[i] != nullptr )
        {
          if ( !_geometries[i]->initialized )
          {
            // Only keep one copy of both index and vertex buffers each.
            _vertexBuffers[i].init( _geometries[i]->vertices, 1, true );
            _indexBuffers[i].init( _geometries[i]->indices, 1, true );

            memAlignedMeshes.resize( _geometries[i]->meshes.size( ) );

            // Textures
            int j = 0;
            for ( const auto& mesh : _geometries[i]->meshes )
            {
              float diffuseTexIndex = -1.0F;

              if ( !mesh.material.diffuseTexPath.empty( ) )
              {
                size_t availablePosition = std::numeric_limits<size_t>::max( );
                bool reuse               = false;

                // Iterate over all textures and find the last spot in the array that does not contain an initialized texture yet.
                // Iterate over all textures and check if the texture can be re-used.
                for ( size_t i = 0; i < _textures.size( ) - 1; ++i )
                {
                  if ( _textures[i] == nullptr )
                  {
                    //RX_WARN( "FOUND AN AVAILABLE POSITION at ", i );
                    availablePosition = i;
                    break;
                  }

                  if ( _textures[i] != nullptr )
                  {
                    if ( _textures[i]->getPath( ) == mesh.material.diffuseTexPath )
                    {
                      // Make sure a new texture will not be created.
                      reuse = true;
                      //RX_WARN( "REUSING: ", mesh.material.diffuseTexPath, " at index: ", i );

                      diffuseTexIndex = static_cast<float>( i );
                      break;
                    }
                  }
                }

                // Create a new texture in case it does not already exist.
                if ( !reuse )
                {
                  if ( availablePosition == std::numeric_limits<size_t>::max( ) )
                  {
                    //RX_WARN( "TRIED TO CREATE A NEW TEXTURE FROM: ", mesh.material.diffuseTexPath, " BUT ARRAY IS OUT OF BOUND." );
                  }
                  else
                  {
                    if ( _textures[availablePosition] == nullptr && !mesh.material.diffuseTexPath.empty( ) )
                    {
                      //RX_WARN( "CREATING NEW TEXTURE at index: ", availablePosition, " from: ", mesh.material.diffuseTexPath );
                      auto texture = std::make_shared<Texture>( );
                      texture->init( mesh.material.diffuseTexPath );
                      _textures[availablePosition] = texture;
                      diffuseTexIndex              = static_cast<float>( availablePosition );
                    }
                  }
                }
              }

              memAlignedMeshes[j] = MeshSSBO( mesh, diffuseTexIndex );
              //RX_WARN( "THE TEXTURE WAS SET AT: ", diffuseTexIndex );

              ++j;
            }

            // Meshes
            _meshBuffers[i].init( memAlignedMeshes );

            _geometries[i]->initialized = true;
            RX_SUCCESS( "Initialized Geometries." );
          }
        }
      }
    }

    RX_SUCCESS( "Uploaded Geometries." );
  }

  void Scene::uploadGeometryInstances( uint32_t imageIndex )
  {
    if ( _settings->_maxGeometryInstancesChanged )
    {
      _settings->_maxGeometryInstancesChanged = false;

      std::vector<GeometryInstanceSSBO> geometryInstances( _settings->_maxGeometryInstances );
      _geometryInstancesBuffer.init( geometryInstances, components::maxResources );

      updateSceneDescriptors( );
    }

    _uploadGeometryInstancesToBuffer = false;

    if ( !_geometryInstances.empty( ) && imageIndex % 2 == 0 ) // @todo % 2 == 0 statement is a temporary fix. Probably a sync error.
    {
      memAlignedGeometryInstances.resize( _geometryInstances.size( ) );
      std::transform( _geometryInstances.begin( ), _geometryInstances.end( ), memAlignedGeometryInstances.begin( ),
                      []( std::shared_ptr<GeometryInstance> instance ) { return GeometryInstanceSSBO { instance->transform,
                                                                                                       instance->transformIT,
                                                                                                       instance->geometryIndex }; } );

      _geometryInstancesBuffer.upload( memAlignedGeometryInstances, imageIndex );

      RX_SUCCESS( "Uploaded geometry instances." );
    }
  }

  void Scene::addDummy( )
  {
    _dummy = true;

    Vertex v1;
    v1.normal = glm::vec3( 0.0F, 1.0F, 0.0F );
    v1.pos    = glm::vec3( -0.00001F, 0.0F, 0.00001F );

    Vertex v2;
    v2.normal = glm::vec3( 0.0F, 1.0F, 0.0F );
    v2.pos    = glm::vec3( 0.00001F, 0.0F, 0.00001F );

    Vertex v3;
    v3.normal = glm::vec3( 0.0F, 1.0F, 0.0F );
    v3.pos    = glm::vec3( 0.00001F, 0.0F, -0.00001F );

    triangle                = std::make_shared<Geometry>( );
    triangle->vertices      = { v1, v2, v3 };
    triangle->indices       = { 0, 1, 2 };
    triangle->geometryIndex = components::geometryIndex++;
    triangle->meshes.push_back( { { }, 0 } );
    triangle->path = "Custom Dummy Triangle";

    triangleInstance = instance( triangle );

    submitGeometry( triangle );
    submitGeometryInstance( triangleInstance );

    RX_VERBOSE( "Scene is empty. Added dummy element." );
  }

  void Scene::removeDummy( )
  {
    if ( triangle != nullptr && _geometryInstances.size( ) > 1 )
    {
      _dummy = false;

      RX_VERBOSE( "Removing dummy element." );
      removeGeometry( triangle );
      triangle = nullptr;
    }
  }

  void Scene::initSceneDescriptorSets( )
  {
    _sceneDescriptors.bindings.reset( );

    // Camera uniform buffer
    _sceneDescriptors.bindings.add( 0, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eRaygenKHR );
    // Scene description buffer
    _sceneDescriptors.bindings.add( 1, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR | vk::ShaderStageFlagBits::eAnyHitKHR );
    // Environment map
    _sceneDescriptors.bindings.add( 2, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eMissKHR );

    _sceneDescriptors.layout = _sceneDescriptors.bindings.initLayoutUnique( );
    _sceneDescriptors.pool   = _sceneDescriptors.bindings.initPoolUnique( components::maxResources );
    _sceneDescriptorsets     = vkCore::initDescriptorSetsUnique( _sceneDescriptors.pool, _sceneDescriptors.layout );
  }

  void Scene::initGeoemtryDescriptorSets( )
  {
    _geometryDescriptors.bindings.reset( );

    // Vertex buffers
    _geometryDescriptors.bindings.add( 0,
                                       vk::DescriptorType::eStorageBuffer,
                                       vk::ShaderStageFlagBits::eClosestHitKHR | vk::ShaderStageFlagBits::eAnyHitKHR,
                                       _settings->_maxGeometry,
                                       vk::DescriptorBindingFlagBits::eUpdateAfterBind );

    // Index buffers
    _geometryDescriptors.bindings.add( 1,
                                       vk::DescriptorType::eStorageBuffer,
                                       vk::ShaderStageFlagBits::eClosestHitKHR | vk::ShaderStageFlagBits::eAnyHitKHR,
                                       _settings->_maxGeometry,
                                       vk::DescriptorBindingFlagBits::eUpdateAfterBind );

    // Mesh buffers
    _geometryDescriptors.bindings.add( 2,
                                       vk::DescriptorType::eStorageBuffer,
                                       vk::ShaderStageFlagBits::eClosestHitKHR | vk::ShaderStageFlagBits::eAnyHitKHR,
                                       _settings->_maxMeshes,
                                       vk::DescriptorBindingFlagBits::eUpdateAfterBind );

    // Textures
    if ( !_immutableSampler )
    {
      _immutableSampler = vk::Initializer::initSamplerUnique( vk::Helper::getSamplerCreateInfo( ) );
    }

    std::vector<vk::Sampler> immutableSamplers( _settings->_maxTextures );
    for ( auto& immutableSampler : immutableSamplers )
    {
      immutableSampler = _immutableSampler.get( );
    }

    _geometryDescriptors.bindings.add( 3,
                                       vk::DescriptorType::eCombinedImageSampler,
                                       vk::ShaderStageFlagBits::eClosestHitKHR | vk::ShaderStageFlagBits::eAnyHitKHR,
                                       _settings->_maxTextures,
                                       vk::DescriptorBindingFlagBits::eUpdateAfterBind,
                                       immutableSamplers.data( ) );

    _geometryDescriptors.layout = _geometryDescriptors.bindings.initLayoutUnique( vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool );
    _geometryDescriptors.pool   = _geometryDescriptors.bindings.initPoolUnique( components::swapchainImageCount, vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind );
    _geometryDescriptorSets     = vkCore::initDescriptorSetsUnique( _geometryDescriptors.pool, _geometryDescriptors.layout );
  }

  void Scene::updateSceneDescriptors( )
  {
    // Environment map
    vk::DescriptorImageInfo environmentMapTextureInfo;
    if ( _environmentMap.getImageView( ) && _environmentMap.getSampler( ) )
    {
      environmentMapTextureInfo.imageLayout = _environmentMap.getLayout( );
      environmentMapTextureInfo.imageView   = _environmentMap.getImageView( );
      environmentMapTextureInfo.sampler     = _environmentMap.getSampler( );
    }
    else
    {
      RX_FATAL( "No default environment map provided." );
    }

    _sceneDescriptors.bindings.writeArray( _sceneDescriptorsets, 0, _cameraUniformBuffer._bufferInfos.data( ) );
    _sceneDescriptors.bindings.writeArray( _sceneDescriptorsets, 1, _geometryInstancesBuffer.getDescriptorInfos( ).data( ) );
    _sceneDescriptors.bindings.write( _sceneDescriptorsets, 2, &environmentMapTextureInfo );
    _sceneDescriptors.bindings.update( );
  }

  void Scene::updateGeoemtryDescriptors( )
  {
    RX_ASSERT( _geometries.size( ) <= _settings->_maxGeometry, "Can not bind more than ", _settings->_maxGeometry, " geometries." );
    RX_ASSERT( _meshBuffers.size( ) <= _settings->_maxMeshes, "Can not bind more than ", _settings->_maxMeshes, " meshes." );
    RX_ASSERT( _vertexBuffers.size( ) == _settings->_maxGeometry, "Vertex buffers container size and geometry limit must be identical." );
    RX_ASSERT( _indexBuffers.size( ) == _settings->_maxGeometry, "Index buffers container size and geometry limit must be identical." );
    RX_ASSERT( _meshBuffers.size( ) == _settings->_maxMeshes, "Mesh buffers container size and mesh limit must be identical." );
    RX_ASSERT( _textures.size( ) == _settings->_maxTextures, "Texture container size and texture limit must be identical." );

    // Vertex buffers infos
    std::vector<vk::DescriptorBufferInfo> vertexBufferInfos;
    vertexBufferInfos.reserve( _vertexBuffers.size( ) );
    for ( const auto& vertexBuffer : _vertexBuffers )
    {
      vk::DescriptorBufferInfo vertexDataBufferInfo( vertexBuffer.get( ).empty( ) ? nullptr : vertexBuffer.get( 0 ),
                                                     0,
                                                     VK_WHOLE_SIZE );

      vertexBufferInfos.push_back( vertexDataBufferInfo );
    }

    // Index buffers infos
    std::vector<vk::DescriptorBufferInfo> indexBufferInfos;
    indexBufferInfos.reserve( _indexBuffers.size( ) );
    for ( const auto& indexBuffer : _indexBuffers )
    {
      vk::DescriptorBufferInfo indexDataBufferInfo( indexBuffer.get( ).empty( ) ? nullptr : indexBuffer.get( 0 ),
                                                    0,
                                                    VK_WHOLE_SIZE );

      indexBufferInfos.push_back( indexDataBufferInfo );
    }

    // Mesh buffers info (each geometry stores n materials and offsets into the array of index buffers)
    std::vector<vk::DescriptorBufferInfo> meshBufferInfos;
    meshBufferInfos.reserve( _meshBuffers.size( ) );
    for ( auto& meshBuffer : _meshBuffers )
    {
      if ( meshBuffer.getDescriptorInfos( ).empty( ) )
      {
        vk::DescriptorBufferInfo meshBufferInfo( nullptr,
                                                 0,
                                                 VK_WHOLE_SIZE );

        meshBufferInfos.push_back( meshBufferInfo );
      }
      else
      {
        meshBufferInfos.push_back( meshBuffer.getDescriptorInfos( )[0] );
      }
    }

    // Texture samplers
    std::vector<vk::DescriptorImageInfo> textureInfos;
    textureInfos.reserve( _textures.size( ) );
    for ( size_t i = 0; i < _settings->_maxTextures; ++i )
    {
      vk::DescriptorImageInfo textureInfo = { };

      if ( _textures[i] != nullptr )
      {
        textureInfo.imageLayout = _textures[i]->getLayout( );
        textureInfo.imageView   = _textures[i]->getImageView( );
        textureInfo.sampler     = _immutableSampler.get( );
      }
      else
      {
        textureInfo.imageLayout = { };
        textureInfo.sampler     = _immutableSampler.get( );
      }

      textureInfos.push_back( textureInfo );
    }

    // Write to and update descriptor bindings
    _geometryDescriptors.bindings.writeArray( _geometryDescriptorSets, 0, vertexBufferInfos.data( ) );
    _geometryDescriptors.bindings.writeArray( _geometryDescriptorSets, 1, indexBufferInfos.data( ) );
    _geometryDescriptors.bindings.writeArray( _geometryDescriptorSets, 2, meshBufferInfos.data( ) );
    _geometryDescriptors.bindings.writeArray( _geometryDescriptorSets, 3, textureInfos.data( ) );

    _geometryDescriptors.bindings.update( );

    int i = 5;
  }

  void Scene::upload( vk::Fence fence, uint32_t imageIndex )
  {
  }
} // namespace RAYEX_NAMESPACE
