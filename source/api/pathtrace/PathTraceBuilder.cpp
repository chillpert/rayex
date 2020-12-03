#include "api/pathtrace/PathTraceBuilder.hpp"

#include "api/Components.hpp"
#include "api/buffers/CommandBuffer.hpp"
#include "api/utility/Helpers.hpp"
#include "api/utility/Initializers.hpp"

namespace RAYEX_NAMESPACE
{
  std::vector<Blas> allDynamicBlas;

  PathTraceBuilder::~PathTraceBuilder( )
  {
    destroy( );
  }

  void PathTraceBuilder::init( )
  {
    auto pipelineProperties          = components::physicalDevice.getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceRayTracingPipelinePropertiesKHR>( );
    _capabilities.pipelineProperties = pipelineProperties.get<vk::PhysicalDeviceRayTracingPipelinePropertiesKHR>( );

    /*
    auto pipelineFeatures          = components::physicalDevice.getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceRayTracingPipelineFeaturesKHR>( );
    _capabilities.pipelineFeatures = pipelineFeatures.get<vk::PhysicalDeviceRayTracingPipelineFeaturesKHR>( );

    auto accelerationStructureProperties          = components::physicalDevice.getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceAccelerationStructurePropertiesKHR>( );
    _capabilities.accelerationStructureProperties = accelerationStructureProperties.get<vk::PhysicalDeviceAccelerationStructurePropertiesKHR>( );

    auto accelerationStructureFeatures          = components::physicalDevice.getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceAccelerationStructureFeaturesKHR>( );
    _capabilities.accelerationStructureFeatures = accelerationStructureFeatures.get<vk::PhysicalDeviceAccelerationStructureFeaturesKHR>( );
    */
  }

  void PathTraceBuilder::destroy( )
  {
    components::device.waitIdle( );

    for ( Blas& blas : _staticBlas_ )
    {
      blas.as.destroy( );
    }

    for ( Blas& blas : _dynamicBlas_ )
    {
      blas.as.destroy( );
    }

    _tlas.as.destroy( );

    _staticBlas_.clear( );
    _dynamicBlas_.clear( );

    _blas_.clear( );
    _indices.clear( );

    for ( Blas& blas : allDynamicBlas )
    {
      blas.as.destroy( );
    }

    allDynamicBlas.clear( );
  }

  auto PathTraceBuilder::modelToBlas( const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer, bool allowTransforms ) const -> Blas
  {
    vk::DeviceAddress vertexAddress = components::device.getBufferAddress( { vertexBuffer.get( ) } );
    vk::DeviceAddress indexAddress  = components::device.getBufferAddress( { indexBuffer.get( ) } );

    vk::AccelerationStructureGeometryTrianglesDataKHR triangles( Vertex::getVertexPositionFormat( ), // vertexFormat
                                                                 vertexAddress,                      // vertexData
                                                                 sizeof( Vertex ),                   // vertexStride
                                                                 vertexBuffer.getCount( ),           // maxVertex
                                                                 vk::IndexType::eUint32,             // indexType
                                                                 indexAddress,                       // indexData
                                                                 { } );                              // transformData

    vk::AccelerationStructureGeometryKHR asGeom( vk::GeometryTypeKHR::eTriangles,    // geometryType
                                                 triangles,                          // geometry
                                                 vk::GeometryFlagBitsKHR::eOpaque ); // flags

    vk::AccelerationStructureBuildRangeInfoKHR offset( indexBuffer.getCount( ) / 3, // primitiveCount
                                                       0,                           // primitiveOffset
                                                       0,                           // firstVertex
                                                       0 );                         // transformOffset

    Blas blas;
    blas.asGeometry.push_back( asGeom );
    blas.asBuildRangeInfo.push_back( offset );

    return blas;
  }

  auto PathTraceBuilder::instanceToVkGeometryInstanceKHR( const BlasInstance& instance ) -> vk::AccelerationStructureInstanceKHR
  {
    Blas& blas { _blas_[instance.blasId] };

    vk::AccelerationStructureDeviceAddressInfoKHR addressInfo( blas.as.as );
    vk::DeviceAddress blasAddress = components::device.getAccelerationStructureAddressKHR( addressInfo );

    glm::mat4 transpose = glm::transpose( instance.transform );

    vk::AccelerationStructureInstanceKHR gInst( { },                 // transform
                                                instance.instanceId, // instanceCustomIndex
                                                instance.mask,       // mask
                                                instance.hitGroupId, // instanceShaderBindingTableRecordOffset
                                                instance.flags,      // flags
                                                blasAddress );       // accelerationStructureReference

    memcpy( &gInst.transform, &transpose, sizeof( gInst.transform ) );

    return gInst;
  }

  void PathTraceBuilder::createBottomLevelAS( const std::vector<VertexBuffer>& vertexBuffers, const std::vector<IndexBuffer>& indexBuffers, const std::vector<std::shared_ptr<Geometry>>& geometries )
  {
    // Clean up previous acceleration structures and free all memory.
    destroy( );

    // BLAS - Storing each primitive in a geometry.
    std::vector<Blas> allStaticBlas;
    allStaticBlas.reserve( vertexBuffers.size( ) );

    allDynamicBlas.reserve( vertexBuffers.size( ) );

    std::vector<uint32_t> staticBlasIndices;
    staticBlasIndices.reserve( vertexBuffers.size( ) );

    std::vector<uint32_t> dynamicBlasIndices;
    dynamicBlasIndices.reserve( vertexBuffers.size( ) );

    for ( size_t i = 0; i < vertexBuffers.size( ); ++i )
    {
      if ( vertexBuffers[i].get( ) && indexBuffers[i].get( ) )
      {
        Blas blas;

        std::pair<size_t, bool> temp;

        // We could add more geometry in each BLAS, but we add only one for now.
        if ( geometries.size( ) > i )
        {
          if ( geometries[i] != nullptr )
          {
            if ( geometries[i]->dynamic )
            {
              blas = modelToBlas( vertexBuffers[i], indexBuffers[i], true );
              allDynamicBlas.emplace_back( blas );
              dynamicBlasIndices.push_back( geometries[i]->geometryIndex );

              temp.first  = allDynamicBlas.size( ) - 1;
              temp.second = true;
            }
            else
            {
              blas = modelToBlas( vertexBuffers[i], indexBuffers[i], false );
              allStaticBlas.emplace_back( blas );
              staticBlasIndices.push_back( geometries[i]->geometryIndex );

              temp.first  = allStaticBlas.size( ) - 1;
              temp.second = false;
            }

            _indices.insert( { geometries[i]->geometryIndex, temp } );
          }
        }
      }
    }

    if ( !allStaticBlas.empty( ) )
    {
      // @todo Check if eAllowUpdate is necessary.
      buildBlas( allStaticBlas, vk::BuildAccelerationStructureFlagBitsKHR::eAllowCompaction | vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace );
      // Keep a copy.
      _staticBlas_ = allStaticBlas;
    }

    if ( !allDynamicBlas.empty( ) )
    {
      buildBlas( allDynamicBlas, vk::BuildAccelerationStructureFlagBitsKHR::eAllowCompaction | vk::BuildAccelerationStructureFlagBitsKHR::eAllowUpdate | vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastBuild );
      // Keep a copy.
      _dynamicBlas_ = allDynamicBlas;
    }

    _blas_.reserve( vertexBuffers.size( ) );

    for ( size_t i = 0; i < vertexBuffers.size( ); ++i )
    {
      auto it = _indices.find( i );
      if ( it != _indices.end( ) )
      {
        // Dynamic
        if ( it->second.second )
        {
          _blas_.emplace_back( _dynamicBlas_[it->second.first] );
        }
        // Static
        else
        {
          _blas_.emplace_back( _staticBlas_[it->second.first] );
        }
      }
    }
  }

  void PathTraceBuilder::buildBlas( std::vector<Blas>& blas_, vk::BuildAccelerationStructureFlagsKHR flags )
  {
    uint32_t blasCount = static_cast<uint32_t>( blas_.size( ) );

    // Is compaction requested?
    bool doCompaction = ( flags & vk::BuildAccelerationStructureFlagBitsKHR::eAllowCompaction ) == vk::BuildAccelerationStructureFlagBitsKHR::eAllowCompaction;

    vk::DeviceSize maxScratch = 0; // Largest scratch buffer for our BLAS

    std::vector<vk::DeviceSize> originalSizes;
    originalSizes.resize( blas_.size( ) );

    std::vector<vk::AccelerationStructureBuildGeometryInfoKHR> buildInfos;
    buildInfos.reserve( blas_.size( ) );

    // Iterate over the groups of geometries, creating one BLAS for each group
    int index = 0;
    for ( Blas& blas : blas_ )
    {
      if ( !blas.as.as )
      {
        components::device.destroyAccelerationStructureKHR( blas.as.as );
      }

      if ( !blas.as.memory )
      {
        components::device.freeMemory( blas.as.memory );
      }

      vk::AccelerationStructureBuildGeometryInfoKHR buildInfo( vk::AccelerationStructureTypeKHR::eBottomLevel,   // type
                                                               flags,                                            // flags
                                                               vk::BuildAccelerationStructureModeKHR::eBuild,    // mode
                                                               nullptr,                                          // srcAccelerationStructure
                                                               { },                                              // dstAccelerationStructure
                                                               static_cast<uint32_t>( blas.asGeometry.size( ) ), // geometryCount
                                                               blas.asGeometry.data( ),                          // pGeometries
                                                               { },                                              // ppGeometries
                                                               { } );                                            // scratchData

      std::vector<uint32_t> maxPrimitiveCount( blas.asBuildRangeInfo.size( ) );

      for ( size_t i = 0; i < blas.asBuildRangeInfo.size( ); ++i )
      {
        maxPrimitiveCount[i] = blas.asBuildRangeInfo[i].primitiveCount;
      }

      vk::AccelerationStructureBuildSizesInfoKHR sizeInfo;
      components::device.getAccelerationStructureBuildSizesKHR( vk::AccelerationStructureBuildTypeKHR::eDevice, &buildInfo, maxPrimitiveCount.data( ), &sizeInfo );

      // Create accleration structure
      // @todo Potentially, pass size and type to initAccelerationStructure and return createInfo instead.
      vk::AccelerationStructureCreateInfoKHR createInfo( { },                                            // createFlags
                                                         { },                                            // buffer
                                                         { },                                            // offset
                                                         sizeInfo.accelerationStructureSize,             // size
                                                         vk::AccelerationStructureTypeKHR::eBottomLevel, // type
                                                         { } );                                          // deviceAddress

      blas.as    = vk::Initializer::initAccelerationStructure( createInfo );
      blas.flags = flags;

      buildInfo.dstAccelerationStructure = blas.as.as;

      maxScratch           = std::max( maxScratch, sizeInfo.buildScratchSize );
      originalSizes[index] = sizeInfo.accelerationStructureSize;

      buildInfos.push_back( buildInfo );

      ++index;
    }

    // Allocate the scratch buffers holding the temporary data of the acceleration structure builder.
    Buffer scratchBuffer( maxScratch,                                                                              // size
                          vk::BufferUsageFlagBits::eShaderDeviceAddress | vk::BufferUsageFlagBits::eStorageBuffer, // usage
                          { components::graphicsFamilyIndex },                                                     // queueFamilyIndices
                          vk::MemoryPropertyFlagBits::eDeviceLocal );                                              // memoryPropertyFlags

    vk::BufferDeviceAddressInfo bufferInfo( scratchBuffer.get( ) );
    vk::DeviceAddress scratchAddress = components::device.getBufferAddress( &bufferInfo );

    // Query size of compact BLAS.
    vk::UniqueQueryPool queryPool = vk::Initializer::initQueryPoolUnique( blasCount, vk::QueryType::eAccelerationStructureCompactedSizeKHR );

    // Create a command buffer containing all the BLAS builds.
    vk::UniqueCommandPool commandPool = vk::Initializer::initCommandPoolUnique( { components::graphicsFamilyIndex } );
    int ctr                           = 0;

    CommandBuffer cmdBuf( commandPool.get( ), blasCount );

    index = 0;
    for ( Blas& blas : blas_ )
    {
      buildInfos[index].scratchData.deviceAddress = scratchAddress;

      std::vector<const vk::AccelerationStructureBuildRangeInfoKHR*> pBuildRangeInfos( blas.asBuildRangeInfo.size( ) );

      size_t infoIndex = 0;
      for ( auto pbuildRangeInfo : pBuildRangeInfos )
      {
        pbuildRangeInfo = &blas.asBuildRangeInfo[infoIndex];
        ++infoIndex;
      }

      cmdBuf.begin( index );

      // Building the acceleration structure
      cmdBuf.get( index ).buildAccelerationStructuresKHR( 1, &buildInfos[index], pBuildRangeInfos.data( ) );

      // Make sure the BLAS were successfully built first before reusing the scratch buffer.
      vk::MemoryBarrier barrier( vk::AccessFlagBits::eAccelerationStructureWriteKHR,  // srcAccessMask
                                 vk::AccessFlagBits::eAccelerationStructureReadKHR ); // dstAccessMask

      cmdBuf.get( index ).pipelineBarrier( vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR, // srcStageMask
                                           vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR, // dstStageMask
                                           { },                                                       // dependencyFlags
                                           1,                                                         // memoryBarrierCount
                                           &barrier,                                                  // pMemoryBarriers
                                           0,                                                         // bufferMemoryBarrierCount
                                           nullptr,                                                   // pBufferMemoryBarriers
                                           0,                                                         // imageMemoryBarrierCount
                                           nullptr );                                                 // pImageMemoryBarriers

      if ( doCompaction )
      {
        cmdBuf.get( index ).writeAccelerationStructuresPropertiesKHR( 1,                                                     // accelerationStructureCount
                                                                      &blas.as.as,                                           // pAccelerationStructures
                                                                      vk::QueryType::eAccelerationStructureCompactedSizeKHR, // queryType
                                                                      queryPool.get( ),                                      // queryPool
                                                                      ctr++ );                                               // firstQuery
      }

      cmdBuf.end( index );

      ++index;
    }

    cmdBuf.submitToQueue( components::graphicsQueue );

    if ( doCompaction )
    {
      CommandBuffer compactionCmdBuf( components::graphicsCmdPool );

      std::vector<vk::DeviceSize> compactSizes( blas_.size( ) );

      auto result = components::device.getQueryPoolResults( queryPool.get( ),                                // queryPool
                                                            0,                                               // firstQuery
                                                            static_cast<uint32_t>( compactSizes.size( ) ),   // queryCount
                                                            compactSizes.size( ) * sizeof( vk::DeviceSize ), // dataSize
                                                            compactSizes.data( ),                            // pData
                                                            sizeof( vk::DeviceSize ),                        // stride
                                                            vk::QueryResultFlagBits::eWait );                // flags

      RX_ASSERT( result == vk::Result::eSuccess, "Failed to get query pool results." );

      std::vector<AccelerationStructure> cleanupAS( blas_.size( ) );

      uint32_t totalOriginalSize = 0;
      uint32_t totalCompactSize  = 0;

      compactionCmdBuf.begin( 0 );

      for ( int i = 0; i < blas_.size( ); ++i )
      {
        totalOriginalSize += static_cast<uint32_t>( originalSizes[i] );
        totalCompactSize += static_cast<uint32_t>( compactSizes[i] );

        // Creating a compact version of the AS.
        vk::AccelerationStructureCreateInfoKHR asCreateInfo( { },                                            // createFlags
                                                             { },                                            // buffer
                                                             { },                                            // offset
                                                             compactSizes[i],                                // size
                                                             vk::AccelerationStructureTypeKHR::eBottomLevel, // type
                                                             { } );                                          // deviceAddress

        auto as = vk::Initializer::initAccelerationStructure( asCreateInfo );

        // Copy the original BLAS to a compact version
        vk::CopyAccelerationStructureInfoKHR copyInfo( blas_[i].as.as,                                   // src
                                                       as.as,                                            // dst
                                                       vk::CopyAccelerationStructureModeKHR::eCompact ); // mode

        compactionCmdBuf.get( 0 ).copyAccelerationStructureKHR( &copyInfo );

        cleanupAS[i] = blas_[i].as;
        blas_[i].as  = as;
      }

      compactionCmdBuf.end( 0 );
      compactionCmdBuf.submitToQueue( components::graphicsQueue );

      for ( auto& as : cleanupAS )
        as.destroy( );

      RX_VERBOSE( "BLAS: Compaction Results: ", totalOriginalSize, " -> ", totalCompactSize, " | Total: ", totalOriginalSize - totalCompactSize );
    }
  }

  void PathTraceBuilder::createTopLevelAS( const std::vector<std::shared_ptr<GeometryInstance>>& geometryInstances )
  {
    instances.clear( );
    instances.reserve( geometryInstances.size( ) );

    for ( uint32_t i = 0; i < geometryInstances.size( ); ++i )
    {
      BlasInstance rayInst;
      rayInst.transform  = geometryInstances[i]->transform;
      rayInst.instanceId = i;
      rayInst.blasId     = geometryInstances[i]->geometryIndex;
      rayInst.hitGroupId = 0; // We will use the same hit group for all objects
      rayInst.flags      = vk::GeometryInstanceFlagBitsKHR::eTriangleFacingCullDisable;

      instances.push_back( rayInst );
    }

    if ( !instances.empty( ) )
    {
      buildTlas( instances, vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace | vk::BuildAccelerationStructureFlagBitsKHR::eAllowUpdate );
    }
  }

  void PathTraceBuilder::buildTlas( const std::vector<BlasInstance>& instances, vk::BuildAccelerationStructureFlagsKHR flags, bool reuse )
  {
    _tlas.flags = flags;

    vk::UniqueCommandPool commandPool = vk::Initializer::initCommandPoolUnique( components::graphicsFamilyIndex );
    CommandBuffer cmdBuf( commandPool.get( ) );

    std::vector<vk::AccelerationStructureInstanceKHR> geometryInstances;
    geometryInstances.reserve( instances.size( ) );

    for ( const auto& instance : instances )
    {
      geometryInstances.push_back( instanceToVkGeometryInstanceKHR( instance ) );
    }

    if ( reuse )
    {
      // destroy geometry instances buffer (probably not necessary in this case because I am using a unique handle)
    }

    _instancesBuffer.init( sizeof( vk::AccelerationStructureInstanceKHR ) * geometryInstances.size( ), vk::BufferUsageFlagBits::eShaderDeviceAddress );

    vk::BufferDeviceAddressInfo bufferInfo( _instancesBuffer.get( ) );
    vk::DeviceAddress instanceAddress = components::device.getBufferAddress( &bufferInfo );

    cmdBuf.begin( 0 );

    vk::MemoryBarrier barrier( vk::AccessFlagBits::eTransferWrite,                   // srcAccessMask
                               vk::AccessFlagBits::eAccelerationStructureWriteKHR ); // dstAccessMask

    cmdBuf.get( 0 ).pipelineBarrier( vk::PipelineStageFlagBits::eTransfer,                      // srcStageMask
                                     vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR, // dstStageMask
                                     { },                                                       // dependencyFlags
                                     1,                                                         // memoryBarrierCount
                                     &barrier,                                                  // pMemoryBarriers
                                     0,                                                         // bufferMemoryBarrierCount
                                     nullptr,                                                   // pBufferMemoryBarriers
                                     0,                                                         // imageMemoryBarrierCount
                                     nullptr );                                                 // pImageMemoryBarriers

    vk::AccelerationStructureGeometryInstancesDataKHR instancesData( VK_FALSE,          // arrayOfPointers
                                                                     instanceAddress ); // data

    vk::AccelerationStructureGeometryKHR tlasGeometry( vk::GeometryTypeKHR::eInstances, // geometryType
                                                       instancesData,                   // geoemtry
                                                       { } );                           // flags

    vk::BuildAccelerationStructureModeKHR mode = reuse ? vk::BuildAccelerationStructureModeKHR::eUpdate : vk::BuildAccelerationStructureModeKHR::eBuild;

    vk::AccelerationStructureBuildGeometryInfoKHR buildInfo( vk::AccelerationStructureTypeKHR::eTopLevel, // type
                                                             flags,                                       // flags
                                                             mode,                                        // mode
                                                             nullptr,                                     // srcAccelerationStructure
                                                             { },                                         // dstAccelerationStructure
                                                             1,                                           // geometryCount
                                                             &tlasGeometry,                               // pGeometries
                                                             { },                                         // ppGeometries
                                                             { } );                                       // scratchData

    uint32_t instancesCount = static_cast<uint32_t>( instances.size( ) );

    vk::AccelerationStructureBuildSizesInfoKHR buildSizesInfo( { },   // accelerationStructureSize
                                                               { },   // updateScratchSize
                                                               { } ); // buildScratchSize

    components::device.getAccelerationStructureBuildSizesKHR( vk::AccelerationStructureBuildTypeKHR::eDevice, &buildInfo, &instancesCount, &buildSizesInfo );

    if ( !reuse )
    {
      vk::AccelerationStructureCreateInfoKHR asCreateInfo( { },                                         // createFlags
                                                           { },                                         // buffer
                                                           { },                                         // offset
                                                           buildSizesInfo.accelerationStructureSize,    // size
                                                           vk::AccelerationStructureTypeKHR::eTopLevel, // type
                                                           { } );                                       // deviceAddress

      _tlas.as = vk::Initializer::initAccelerationStructure( asCreateInfo );
    }

    Buffer scratchBuffer( buildSizesInfo.buildScratchSize,                                                                           // size
                          vk::BufferUsageFlagBits::eAccelerationStructureStorageKHR | vk::BufferUsageFlagBits::eShaderDeviceAddress, // usage
                          { components::graphicsFamilyIndex },                                                                       // queueFamilyIndices
                          vk::MemoryPropertyFlagBits::eDeviceLocal );                                                                // memoryPropertyFlags

    bufferInfo.buffer = scratchBuffer.get( );

    vk::DeviceAddress scratchAddress = components::device.getBufferAddress( &bufferInfo );

    buildInfo.srcAccelerationStructure  = reuse ? _tlas.as.as : nullptr;
    buildInfo.dstAccelerationStructure  = _tlas.as.as;
    buildInfo.scratchData.deviceAddress = scratchAddress;

    vk::AccelerationStructureBuildRangeInfoKHR buildRangeInfo( instancesCount, // primitiveCount
                                                               0,              // primitiveOffset
                                                               0,              // firstVertex
                                                               0 );            // transformOffset
    const vk::AccelerationStructureBuildRangeInfoKHR* pBuildRangeInfo = &buildRangeInfo;

    cmdBuf.get( 0 ).buildAccelerationStructuresKHR( 1, &buildInfo, &pBuildRangeInfo );

    cmdBuf.end( 0 );
    cmdBuf.submitToQueue( components::graphicsQueue );
  }

  void PathTraceBuilder::createStorageImage( vk::Extent2D swapchainExtent )
  {
    auto storageImageInfo   = vk::Helper::getImageCreateInfo( vk::Extent3D( swapchainExtent.width, swapchainExtent.height, 1 ) );
    storageImageInfo.usage  = vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eStorage;
    storageImageInfo.format = vk::Format::eB8G8R8A8Unorm; // TODO: make this the surface format, and not hard-coded

    _storageImage.init( storageImageInfo );
    _storageImage.transitionToLayout( vk::ImageLayout::eGeneral );

    _storageImageView = vk::Initializer::initImageViewUnique( vk::Helper::getImageViewCreateInfo( _storageImage.get( ), _storageImage.getFormat( ) ) );
  }

  void PathTraceBuilder::createShaderBindingTable( )
  {
    uint32_t groupHandleSize = _capabilities.pipelineProperties.shaderGroupHandleSize;
    uint32_t baseAlignment   = _capabilities.pipelineProperties.shaderGroupBaseAlignment;

    uint32_t sbtSize = _shaderGroups * baseAlignment;

    _sbtBuffer.init( sbtSize,
                     vk::BufferUsageFlagBits::eTransferSrc,
                     { components::graphicsFamilyIndex },
                     vk::MemoryPropertyFlagBits::eHostVisible );

    std::vector<uint8_t> shaderHandleStorage( sbtSize );
    auto result = components::device.getRayTracingShaderGroupHandlesKHR( _pipeline.get( ),
                                                                         0,
                                                                         _shaderGroups,
                                                                         sbtSize,
                                                                         shaderHandleStorage.data( ) );

    RX_ASSERT( result == vk::Result::eSuccess, "Failed to get ray tracing shader group handles." );

    void* mapped = NULL;
    result       = components::device.mapMemory( _sbtBuffer.getMemory( ), 0, _sbtBuffer.getSize( ), { }, &mapped );

    RX_ASSERT( result == vk::Result::eSuccess, "Failed to map memory for shader binding table." );

    auto* pData = reinterpret_cast<uint8_t*>( mapped );
    for ( uint32_t i = 0; i < _shaderGroups; ++i )
    {
      memcpy( pData, shaderHandleStorage.data( ) + i * groupHandleSize, groupHandleSize ); // raygen
      pData += baseAlignment;
    }

    components::device.unmapMemory( _sbtBuffer.getMemory( ) );
  }

  void PathTraceBuilder::createPipeline( const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts, Settings* settings )
  {
    // Check if selected recursion depth exceeds maximum supported value.
    auto recursionDepth = settings->getRecursionDepth( );
    if ( recursionDepth > _capabilities.pipelineProperties.maxRayRecursionDepth )
    {
      RX_WARN( "Selected recursion depth of ", recursionDepth, " exceeds maximum of ", _capabilities.pipelineProperties.maxRayRecursionDepth, ". Using maximum value instead." );
      settings->setRecursionDepth( _capabilities.pipelineProperties.maxRayRecursionDepth );
    }

    //uint32_t anticipatedDirectionalLights = settings->maxDirectionalLights.has_value( ) ? settings->maxDirectionalLights.value( ) : components::maxDirectionalLights;
    //uint32_t anticipatedPointLights       = settings->maxPointLights.has_value( ) ? settings->maxPointLights.value( ) : components::maxPointLights;
    //Util::processShaderMacros( "shaders/PathTrace.rchit", anticipatedDirectionalLights, anticipatedPointLights, components::modelCount );

    auto rgen       = vk::Initializer::initShaderModuleUnique( "shaders/PathTrace.rgen" );
    auto miss       = vk::Initializer::initShaderModuleUnique( "shaders/PathTrace.rmiss" );
    auto chit       = vk::Initializer::initShaderModuleUnique( "shaders/PathTrace.rchit" );
    auto missShadow = vk::Initializer::initShaderModuleUnique( "shaders/PathTraceShadow.rmiss" );

    vk::PushConstantRange ptPushConstant( vk::ShaderStageFlagBits::eRaygenKHR | vk::ShaderStageFlagBits::eMissKHR | vk::ShaderStageFlagBits::eClosestHitKHR, // stageFlags
                                          0,                                                                                                                 // offset
                                          sizeof( PtPushConstants ) );                                                                                       // size

    std::array<vk::PushConstantRange, 1> pushConstantRanges = { ptPushConstant };

    vk::PipelineLayoutCreateInfo layoutInfo( { },                                                   // flags
                                             static_cast<uint32_t>( descriptorSetLayouts.size( ) ), // setLayoutCount
                                             descriptorSetLayouts.data( ),                          // pSetLayouts
                                             static_cast<uint32_t>( pushConstantRanges.size( ) ),   // pushConstantRangeCount
                                             pushConstantRanges.data( ) );                          // pPushConstantRanges

    _layout = components::device.createPipelineLayoutUnique( layoutInfo );
    RX_ASSERT( _layout.get( ), "Failed to create pipeline layout for path tracing pipeline." );

    std::array<vk::PipelineShaderStageCreateInfo, 4> shaderStages;
    shaderStages[0] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eRaygenKHR, rgen.get( ) );
    shaderStages[1] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eMissKHR, miss.get( ) );
    shaderStages[2] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eMissKHR, missShadow.get( ) );
    shaderStages[3] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eClosestHitKHR, chit.get( ) );

    // Set up path tracing shader groups.
    std::array<vk::RayTracingShaderGroupCreateInfoKHR, 4> groups;

    for ( auto& group : groups )
    {
      group.generalShader      = VK_SHADER_UNUSED_KHR;
      group.closestHitShader   = VK_SHADER_UNUSED_KHR;
      group.anyHitShader       = VK_SHADER_UNUSED_KHR;
      group.intersectionShader = VK_SHADER_UNUSED_KHR;
    }

    groups[0].generalShader = 0;
    groups[0].type          = vk::RayTracingShaderGroupTypeKHR::eGeneral;

    groups[1].generalShader = 1;
    groups[1].type          = vk::RayTracingShaderGroupTypeKHR::eGeneral;

    groups[2].generalShader = 2;
    groups[2].type          = vk::RayTracingShaderGroupTypeKHR::eGeneral;

    groups[3].closestHitShader = 3;
    groups[3].type             = vk::RayTracingShaderGroupTypeKHR::eTrianglesHitGroup;

    _shaderGroups = static_cast<uint32_t>( groups.size( ) );

    vk::RayTracingPipelineCreateInfoKHR createInfo( { },                                           // flags
                                                    static_cast<uint32_t>( shaderStages.size( ) ), // stageCount
                                                    shaderStages.data( ),                          // pStages
                                                    static_cast<uint32_t>( groups.size( ) ),       // groupCount
                                                    groups.data( ),                                // pGroups
                                                    settings->getRecursionDepth( ),                // maxPipelineRayRecursionDepth
                                                    { },                                           // pLibraryInfo
                                                    nullptr,                                       // pLibraryInterface
                                                    { },                                           // pDynamicState
                                                    _layout.get( ),                                // layout
                                                    { },                                           // basePipelineHandle
                                                    0 );                                           // basePipelineIndex

    _pipeline = components::device.createRayTracingPipelineKHRUnique( { }, nullptr, createInfo );
    RX_ASSERT( _pipeline.get( ), "Failed to create path tracing pipeline." );
  }

  void PathTraceBuilder::pathTrace( vk::CommandBuffer swapchainCommandBuffer, vk::Image swapchainImage, vk::Extent2D extent )
  {
    vk::DeviceSize progSize = _capabilities.pipelineProperties.shaderGroupBaseAlignment;
    vk::DeviceSize sbtSize  = progSize * static_cast<vk::DeviceSize>( _shaderGroups );

    vk::DeviceAddress sbtAddress = components::device.getBufferAddress( _sbtBuffer.get( ) );

    vk::DeviceSize rayGenOffset        = 0U * progSize; // Start at the beginning of _sbtBuffer
    vk::DeviceSize missOffset          = 1U * progSize; // Jump over raygen
    vk::DeviceSize chitGroupOffset     = 3U * progSize; // Jump over the previous two miss shaders
    vk::DeviceSize callableGroupOffset = 4U * progSize;

    vk::StridedDeviceAddressRegionKHR bufferRegionRayGen( sbtAddress + rayGenOffset, // deviceAddress
                                                          progSize,                  // stride
                                                          sbtSize );                 // size

    vk::StridedDeviceAddressRegionKHR bufferRegionMiss( sbtAddress + missOffset, // deviceAddress
                                                        progSize,                // stride
                                                        sbtSize );               // size

    vk::StridedDeviceAddressRegionKHR bufferRegionChit( sbtAddress + chitGroupOffset, // deviceAddress
                                                        progSize,                     // stride
                                                        sbtSize );                    // size

    vk::StridedDeviceAddressRegionKHR callableShaderBindingTable( sbtAddress + callableGroupOffset, // deviceAddress
                                                                  progSize,                         // stride
                                                                  sbtSize );                        // size

    swapchainCommandBuffer.traceRaysKHR( &bufferRegionRayGen,         // pRaygenShaderBindingTable
                                         &bufferRegionMiss,           // pMissShaderBindingTable
                                         &bufferRegionChit,           // pHitShaderBindingTable
                                         &callableShaderBindingTable, // pCallableShaderBindingTable
                                         extent.width,                // width
                                         extent.height,               // height
                                         1 );                         // depth

    // Image layout transitions for copying.
    vk::Helper::transitionImageLayout( swapchainImage, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, swapchainCommandBuffer );
    _storageImage.transitionToLayout( vk::ImageLayout::eTransferSrcOptimal, swapchainCommandBuffer );

    // Copy path tracing output (storage image) to swapchain image.
    vk::ImageCopy copyRegion( { vk::ImageAspectFlagBits::eColor, 0, 0, 1 }, // srcSubresource
                              { 0, 0, 0 },                                  // srcOffset
                              { vk::ImageAspectFlagBits::eColor, 0, 0, 1 }, // dstSubresource
                              { 0, 0, 0 },                                  // dstOffset
                              { extent.width, extent.height, 1 } );         // extent

    swapchainCommandBuffer.copyImage( _storageImage.get( ),                 // srcImage
                                      vk::ImageLayout::eTransferSrcOptimal, // srcImageLayout
                                      swapchainImage,                       // dstImage
                                      vk::ImageLayout::eTransferDstOptimal, // dstImageLayout
                                      1,                                    // regionCount
                                      &copyRegion );                        // pRegions

    // Undo image layout transitions.
    vk::Helper::transitionImageLayout( swapchainImage, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::ePresentSrcKHR, swapchainCommandBuffer );
    _storageImage.transitionToLayout( vk::ImageLayout::eGeneral, swapchainCommandBuffer );
  }
} // namespace RAYEX_NAMESPACE
