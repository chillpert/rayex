#include "api/raytracing/RayTracingBuilder.hpp"

#include "api/Components.hpp"
#include "api/buffers/CommandBuffer.hpp"
#include "api/utility/Helpers.hpp"
#include "api/utility/Initializers.hpp"

namespace RAYEX_NAMESPACE
{
  RayTracingBuilder::~RayTracingBuilder( )
  {
    destroy( );
  }

  void RayTracingBuilder::init( )
  {
    auto properties = components::physicalDevice.getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceRayTracingPropertiesKHR>( );
    _rtProperties   = properties.get<vk::PhysicalDeviceRayTracingPropertiesKHR>( );
  }

  void RayTracingBuilder::destroy( )
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

    /*
    for ( Blas& blas : _blas_ )
    {
      blas.as.destroy( );
    }
    */

    _tlas.as.destroy( );

    _staticBlas_.clear( );
    _dynamicBlas_.clear( );

    _blas_.clear( );
    _indices.clear( );
  }

  auto RayTracingBuilder::modelToBlas( const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer, bool allowTransforms ) const -> Blas
  {
    vk::AccelerationStructureCreateGeometryTypeInfoKHR asCreate( vk::GeometryTypeKHR::eTriangles,    // geometryType
                                                                 indexBuffer.getCount( ) / 3,        // maxPrimitiveCount
                                                                 vk::IndexType::eUint32,             // indexType
                                                                 vertexBuffer.getCount( ),           // maxVertexCount
                                                                 Vertex::getVertexPositionFormat( ), // vertexFormat
                                                                 allowTransforms );                  // allowsTransforms

    vk::DeviceAddress vertexAddress = components::device.getBufferAddress( { vertexBuffer.get( ) } );
    vk::DeviceAddress indexAddress  = components::device.getBufferAddress( { indexBuffer.get( ) } );

    vk::AccelerationStructureGeometryTrianglesDataKHR triangles( asCreate.vertexFormat, // vertexFormat
                                                                 vertexAddress,         // vertexData
                                                                 sizeof( Vertex ),      // vertexStride
                                                                 asCreate.indexType,    // indexType
                                                                 indexAddress,          // indexData
                                                                 { } );                 // transformData

    vk::AccelerationStructureGeometryKHR asGeom( asCreate.geometryType,              // geometryType
                                                 triangles,                          // geometry
                                                 vk::GeometryFlagBitsKHR::eOpaque ); // flags

    vk::AccelerationStructureBuildOffsetInfoKHR offset( asCreate.maxPrimitiveCount, // primitiveCount
                                                        0,                          // primitiveOffset
                                                        0,                          // firstVertex
                                                        0 );                        // transformOffset

    Blas blas;
    blas.asGeometry.push_back( asGeom );
    blas.asCreateGeometryInfo.push_back( asCreate );
    blas.asBuildOffsetInfo.push_back( offset );

    return blas;
  }

  auto RayTracingBuilder::instanceToVkGeometryInstanceKHR( const BlasInstance& instance ) -> vk::AccelerationStructureInstanceKHR
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

  void RayTracingBuilder::createBottomLevelAS( const std::vector<VertexBuffer>& vertexBuffers, const std::vector<IndexBuffer>& indexBuffers, const std::vector<std::shared_ptr<Geometry>>& geometries )
  {
    // Clean up previous acceleration structures and free all memory.
    destroy( );

    // BLAS - Storing each primitive in a geometry.
    std::vector<Blas> allStaticBlas;
    allStaticBlas.reserve( vertexBuffers.size( ) );

    std::vector<Blas> allDynamicBlas;
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

  void RayTracingBuilder::buildBlas( std::vector<Blas>& blas_, vk::BuildAccelerationStructureFlagsKHR flags )
  {
    vk::DeviceSize maxScratch = 0;

    // Is compaction requested?
    bool doCompaction = ( flags & vk::BuildAccelerationStructureFlagBitsKHR::eAllowCompaction ) == vk::BuildAccelerationStructureFlagBitsKHR::eAllowCompaction;

    std::vector<vk::DeviceSize> originalSizes;
    originalSizes.resize( blas_.size( ) );

    // Iterate over the groups of geometries, creating one BLAS for each group
    int index = 0;
    for ( Blas& blas : blas_ )
    {
      vk::AccelerationStructureCreateInfoKHR asCreateInfo( { },                                                        // compactedSize
                                                           vk::AccelerationStructureTypeKHR::eBottomLevel,             // type
                                                           flags,                                                      // flags
                                                           static_cast<uint32_t>( blas.asCreateGeometryInfo.size( ) ), // maxGeometryCount
                                                           blas.asCreateGeometryInfo.data( ),                          // pGeometryInfos
                                                           { } );                                                      // deviceAddress

      blas.as    = vk::Initializer::initAccelerationStructure( asCreateInfo );
      blas.flags = flags;

      vk::AccelerationStructureMemoryRequirementsInfoKHR memInfo( vk::AccelerationStructureMemoryRequirementsTypeKHR::eBuildScratch, // type
                                                                  vk::AccelerationStructureBuildTypeKHR::eDevice,                    // buildType
                                                                  blas.as.as );                                                      // accelerationStructure

      vk::MemoryRequirements2 memoryRequirements = RAYEX_NAMESPACE::components::device.getAccelerationStructureMemoryRequirementsKHR( memInfo );
      vk::DeviceSize scratchSize                 = memoryRequirements.memoryRequirements.size;

      maxScratch = std::max( maxScratch, scratchSize );

      memInfo.type       = vk::AccelerationStructureMemoryRequirementsTypeKHR::eObject;
      memoryRequirements = RAYEX_NAMESPACE::components::device.getAccelerationStructureMemoryRequirementsKHR( memInfo );

      originalSizes[index] = memoryRequirements.memoryRequirements.size;

      ++index;
    }

    // Allocate the scratch buffers holding the temporary data of the acceleration structure builder.
    vk::MemoryAllocateFlagsInfo allocateFlags( vk::MemoryAllocateFlagBitsKHR::eDeviceAddress );

    Buffer scratchBuffer( maxScratch,                                                                              // size
                          vk::BufferUsageFlagBits::eRayTracingKHR | vk::BufferUsageFlagBits::eShaderDeviceAddress, // usage
                          { components::graphicsFamilyIndex },                                                     // queueFamilyIndices
                          vk::MemoryPropertyFlagBits::eDeviceLocal,                                                // memoryPropertyFlags
                          &allocateFlags );                                                                        // pNextMemory

    vk::BufferDeviceAddressInfo bufferInfo( scratchBuffer.get( ) );
    vk::DeviceAddress scratchAddress = components::device.getBufferAddress( bufferInfo );

    // Query size of compact BLAS.
    vk::UniqueQueryPool queryPool = vk::Initializer::initQueryPoolUnique( static_cast<uint32_t>( blas_.size( ) ), vk::QueryType::eAccelerationStructureCompactedSizeKHR );

    // Create a command buffer containing all the BLAS builds.
    vk::UniqueCommandPool commandPool = vk::Initializer::initCommandPoolUnique( { components::graphicsFamilyIndex } );
    int ctr                           = 0;

    CommandBuffer cmdBuf( commandPool.get( ), static_cast<uint32_t>( blas_.size( ) ) );

    index = 0;
    for ( Blas& blas : blas_ )
    {
      const vk::AccelerationStructureGeometryKHR* pGeometry = blas.asGeometry.data( );

      vk::AccelerationStructureBuildGeometryInfoKHR bottomAsInfo( vk::AccelerationStructureTypeKHR::eBottomLevel,   // type
                                                                  flags,                                            // flags
                                                                  VK_FALSE,                                         // update
                                                                  nullptr,                                          // srcAccelerationStructure
                                                                  blas.as.as,                                       // dstAccelerationStructure
                                                                  VK_FALSE,                                         // geometryArrayOfPointers
                                                                  static_cast<uint32_t>( blas.asGeometry.size( ) ), // geometryCount
                                                                  &pGeometry,                                       // ppGeometries
                                                                  scratchAddress );                                 // scratchData

      // Pointers of offset.
      std::vector<const vk::AccelerationStructureBuildOffsetInfoKHR*> pBuildOffset( blas.asBuildOffsetInfo.size( ) );
      for ( size_t i = 0; i < blas.asBuildOffsetInfo.size( ); ++i )
        pBuildOffset[i] = &blas.asBuildOffsetInfo[i];

      cmdBuf.begin( index );

      // Building the AS.
      cmdBuf.get( index ).buildAccelerationStructureKHR( 1, &bottomAsInfo, pBuildOffset.data( ) );

      // Since the scratch buffer is reused across builds, we need a barrier to ensure one build is finished before starting the next one.
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

      components::device.getQueryPoolResults( queryPool.get( ),                                // queryPool
                                              0,                                               // firstQuery
                                              static_cast<uint32_t>( compactSizes.size( ) ),   // queryCount
                                              compactSizes.size( ) * sizeof( vk::DeviceSize ), // dataSize
                                              compactSizes.data( ),                            // pData
                                              sizeof( vk::DeviceSize ),                        // stride
                                              vk::QueryResultFlagBits::eWait );                // flags

      std::vector<AccelerationStructure> cleanupAS( blas_.size( ) );

      uint32_t totalOriginalSize = 0;
      uint32_t totalCompactSize  = 0;

      compactionCmdBuf.begin( 0 );

      for ( int i = 0; i < blas_.size( ); ++i )
      {
        totalOriginalSize += static_cast<uint32_t>( originalSizes[i] );
        totalCompactSize += static_cast<uint32_t>( compactSizes[i] );

        // Creating a compact version of the AS.
        vk::AccelerationStructureCreateInfoKHR asCreateInfo( compactSizes[i],                                // compactedSize
                                                             vk::AccelerationStructureTypeKHR::eBottomLevel, // type
                                                             flags,                                          // flags
                                                             { },                                            // maxGeometryCount
                                                             { },                                            // pGeometryInfos
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

  void RayTracingBuilder::createTopLevelAS( const std::vector<std::shared_ptr<GeometryInstance>>& geometryInstances )
  {
    std::vector<BlasInstance> instances;
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

  void RayTracingBuilder::buildTlas( const std::vector<BlasInstance>& instances, vk::BuildAccelerationStructureFlagsKHR flags )
  {
    _tlas.flags = flags;

    vk::AccelerationStructureCreateGeometryTypeInfoKHR geometryCreate( vk::GeometryTypeKHR::eInstances,            // geometryType
                                                                       static_cast<uint32_t>( instances.size( ) ), // maxPrimitiveCount
                                                                       { },                                        // indexType
                                                                       { },                                        // maxVertexCount
                                                                       { },                                        // vertexFormat
                                                                       VK_TRUE );                                  // allowsTransforms

    vk::AccelerationStructureCreateInfoKHR asCreateInfo( { },                                         // compactedSize
                                                         vk::AccelerationStructureTypeKHR::eTopLevel, // type
                                                         flags,                                       // flags
                                                         1,                                           // maxGeometryCount
                                                         &geometryCreate,                             // pGeometryInfos
                                                         { } );                                       // deviceAddress

    _tlas.as = vk::Initializer::initAccelerationStructure( asCreateInfo );

    // TODO: Just like the BLAS creation we need a scratch buffer. We could save one allocation by re-using the BLAS scratch buffer.
    vk::AccelerationStructureMemoryRequirementsInfoKHR memoryRequirementsInfo( vk::AccelerationStructureMemoryRequirementsTypeKHR::eBuildScratch, // type
                                                                               vk::AccelerationStructureBuildTypeKHR::eDevice,                    // buildType
                                                                               _tlas.as.as );                                                     // accelerationStructure

    vk::MemoryRequirements2 reqMem = components::device.getAccelerationStructureMemoryRequirementsKHR( memoryRequirementsInfo );
    vk::DeviceSize scratchSize     = reqMem.memoryRequirements.size;

    // Allocate the scratch buffers holding the temporary data of the acceleration structure builder.
    vk::MemoryAllocateFlagsInfo allocateFlags( vk::MemoryAllocateFlagBitsKHR::eDeviceAddress );

    Buffer scratchBuffer( scratchSize,                                                                             // size
                          vk::BufferUsageFlagBits::eRayTracingKHR | vk::BufferUsageFlagBits::eShaderDeviceAddress, // usage
                          { components::graphicsFamilyIndex },                                                     // queueFamilyIndices
                          vk::MemoryPropertyFlagBits::eDeviceLocal,                                                // memoryPropertyFlags
                          &allocateFlags );                                                                        // pNextMemory

    vk::DeviceAddress scratchAddress = components::device.getBufferAddress( { scratchBuffer.get( ) } );

    std::vector<vk::AccelerationStructureInstanceKHR> geometryInstances;
    geometryInstances.reserve( instances.size( ) );

    for ( const auto& instance : instances )
    {
      geometryInstances.push_back( instanceToVkGeometryInstanceKHR( instance ) );
    }

    // Building the TLAS.
    vk::UniqueCommandPool commandPool = vk::Initializer::initCommandPoolUnique( components::graphicsFamilyIndex );
    CommandBuffer cmdBuf( commandPool.get( ) );

    // Create a buffer holding the actual instance data for use by the AS builder.
    vk::DeviceSize instanceDescsSizeInBytes = instances.size( ) * sizeof( vk::AccelerationStructureInstanceKHR );

    // Allocate the instance buffer and copy its contents from host to device memory.
    _instanceBuffer.init( instanceDescsSizeInBytes,                                                                // size
                          vk::BufferUsageFlagBits::eRayTracingKHR | vk::BufferUsageFlagBits::eShaderDeviceAddress, // usage
                          { components::graphicsFamilyIndex },                                                     // queueFamilyIndices
                          vk::MemoryPropertyFlagBits::eHostVisible,                                                // memoryPropertyFlags
                          &allocateFlags );                                                                        // pNextMemory

    _instanceBuffer.fill<vk::AccelerationStructureInstanceKHR>( geometryInstances );

    vk::DeviceAddress instanceAddress = components::device.getBufferAddress( { _instanceBuffer.get( ) } );

    // Make sure the copy of the instance buffer are copied before triggering the acceleration structure build.
    vk::MemoryBarrier barrier( vk::AccessFlagBits::eTransferWrite,                   // srcAccessMask
                               vk::AccessFlagBits::eAccelerationStructureWriteKHR ); // dstAccessMask

    cmdBuf.begin( 0 );

    cmdBuf.get( 0 ).pipelineBarrier( vk::PipelineStageFlagBits::eTransfer,                      // srcStageMask
                                     vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR, // dstStageMask
                                     { },                                                       // dependencyFlags
                                     1,                                                         // memoryBarrierCount
                                     &barrier,                                                  // pMemoryBarriers
                                     0,                                                         // bufferMemoryBarrierCount
                                     nullptr,                                                   // pBufferMemoryBarriers
                                     0,                                                         // imageMemoryBarrierCount
                                     nullptr );                                                 // pImageMemoryBarriers

    // Build the TLAS.
    vk::AccelerationStructureGeometryDataKHR geometry;
    geometry.instances.arrayOfPointers    = VK_FALSE;
    geometry.instances.data.deviceAddress = instanceAddress;

    vk::AccelerationStructureGeometryKHR topAsGeometry( vk::GeometryTypeKHR::eInstances, // geometryType
                                                        geometry,                        // geometry
                                                        { } );                           // flags

    const vk::AccelerationStructureGeometryKHR* pGeometry = &topAsGeometry;

    vk::AccelerationStructureBuildGeometryInfoKHR topAsInfo( vk::AccelerationStructureTypeKHR::eTopLevel, // type
                                                             flags,                                       // flags
                                                             VK_FALSE,                                    // update
                                                             nullptr,                                     // srcAccelerationStructure
                                                             _tlas.as.as,                                 // dstAccelerationStructure
                                                             VK_FALSE,                                    // geometryArrayOfPointers
                                                             1,                                           // geometryCount
                                                             &pGeometry,                                  // ppGeometries
                                                             scratchAddress );                            // scratchData

    // Build Offsets info: n instances.
    vk::AccelerationStructureBuildOffsetInfoKHR buildOffsetInfo( static_cast<uint32_t>( instances.size( ) ), // primitiveCount
                                                                 0,                                          // primitiveOffset
                                                                 0,                                          // firstVertex
                                                                 0 );                                        // transformOffset

    const vk::AccelerationStructureBuildOffsetInfoKHR* pBuildOffsetInfo = &buildOffsetInfo;

    // Build the TLAS.
    cmdBuf.get( 0 ).buildAccelerationStructureKHR( 1, &topAsInfo, &pBuildOffsetInfo );

    cmdBuf.end( 0 );
    cmdBuf.submitToQueue( components::graphicsQueue );
  }

  void RayTracingBuilder::createStorageImage( vk::Extent2D swapchainExtent )
  {
    auto storageImageInfo   = vk::Helper::getImageCreateInfo( vk::Extent3D( swapchainExtent.width, swapchainExtent.height, 1 ) );
    storageImageInfo.usage  = vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eStorage;
    storageImageInfo.format = vk::Format::eB8G8R8A8Unorm; // TODO: make this the surface format, and not hard-coded

    _storageImage.init( storageImageInfo );
    _storageImage.transitionToLayout( vk::ImageLayout::eGeneral );

    _storageImageView = vk::Initializer::initImageViewUnique( vk::Helper::getImageViewCreateInfo( _storageImage.get( ), _storageImage.getFormat( ) ) );
  }

  void RayTracingBuilder::createShaderBindingTable( )
  {
    uint32_t groupHandleSize = _rtProperties.shaderGroupHandleSize;
    uint32_t baseAlignment   = _rtProperties.shaderGroupBaseAlignment;

    uint32_t sbtSize = _shaderGroups * baseAlignment;

    _sbtBuffer.init( sbtSize,
                     vk::BufferUsageFlagBits::eTransferSrc,
                     { components::graphicsFamilyIndex },
                     vk::MemoryPropertyFlagBits::eHostVisible );

    std::vector<uint8_t> shaderHandleStorage( sbtSize );
    components::device.getRayTracingShaderGroupHandlesKHR( _pipeline.get( ),
                                                           0,
                                                           _shaderGroups,
                                                           sbtSize,
                                                           shaderHandleStorage.data( ) );

    void* mapped = NULL;
    components::device.mapMemory( _sbtBuffer.getMemory( ), 0, _sbtBuffer.getSize( ), { }, &mapped );

    auto* pData = reinterpret_cast<uint8_t*>( mapped );
    for ( uint32_t i = 0; i < _shaderGroups; ++i )
    {
      memcpy( pData, shaderHandleStorage.data( ) + i * groupHandleSize, groupHandleSize ); // raygen
      pData += baseAlignment;
    }

    components::device.unmapMemory( _sbtBuffer.getMemory( ) );
  }

  void RayTracingBuilder::createPipeline( const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts, const Settings* settings )
  {
    //uint32_t anticipatedDirectionalLights = settings->maxDirectionalLights.has_value( ) ? settings->maxDirectionalLights.value( ) : components::maxDirectionalLights;
    //uint32_t anticipatedPointLights       = settings->maxPointLights.has_value( ) ? settings->maxPointLights.value( ) : components::maxPointLights;
    //Util::processShaderMacros( "shaders/raytrace.rchit", anticipatedDirectionalLights, anticipatedPointLights, components::modelCount );

    auto rgen       = vk::Initializer::initShaderModuleUnique( "shaders/raytrace.rgen" );
    auto miss       = vk::Initializer::initShaderModuleUnique( "shaders/raytrace.rmiss" );
    auto chit       = vk::Initializer::initShaderModuleUnique( "shaders/raytrace.rchit" );
    auto missShadow = vk::Initializer::initShaderModuleUnique( "shaders/raytraceShadow.rmiss" );

    vk::PushConstantRange rtPushConstant( vk::ShaderStageFlagBits::eRaygenKHR | vk::ShaderStageFlagBits::eMissKHR | vk::ShaderStageFlagBits::eClosestHitKHR, // stageFlags
                                          0,                                                                                                                 // offset
                                          sizeof( RayTracePushConstants ) );                                                                                 // size

    std::array<vk::PushConstantRange, 1> pushConstantRanges = { rtPushConstant };

    vk::PipelineLayoutCreateInfo layoutInfo( { },                                                   // flags
                                             static_cast<uint32_t>( descriptorSetLayouts.size( ) ), // setLayoutCount
                                             descriptorSetLayouts.data( ),                          // pSetLayouts
                                             static_cast<uint32_t>( pushConstantRanges.size( ) ),   // pushConstantRangeCount
                                             pushConstantRanges.data( ) );                          // pPushConstantRanges

    _layout = components::device.createPipelineLayoutUnique( layoutInfo );
    RX_ASSERT( _layout.get( ), "Failed to create pipeline layout for ray tracing pipeline." );

    std::array<vk::PipelineShaderStageCreateInfo, 4> shaderStages;
    shaderStages[0] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eRaygenKHR, rgen.get( ) );
    shaderStages[1] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eMissKHR, miss.get( ) );
    shaderStages[2] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eMissKHR, missShadow.get( ) );
    shaderStages[3] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eClosestHitKHR, chit.get( ) );

    // Set up raytracing shader groups.
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
                                                    settings->getRecursionDepth( ),                // maxRecursionDepth
                                                    0,                                             // libraries
                                                    nullptr,                                       // pLibraryInterface
                                                    _layout.get( ),                                // layout
                                                    nullptr,                                       // basePipelineHandle
                                                    0 );                                           // basePipelineIndex

    _pipeline = components::device.createRayTracingPipelineKHRUnique( nullptr, createInfo );
    RX_ASSERT( _pipeline.get( ), "Failed to create ray tracing pipeline." );
  }

  void RayTracingBuilder::rayTrace( vk::CommandBuffer swapchainCommandBuffer, vk::Image swapchainImage, vk::Extent2D extent )
  {
    vk::DeviceSize progSize = _rtProperties.shaderGroupBaseAlignment;
    vk::DeviceSize sbtSize  = progSize * static_cast<vk::DeviceSize>( _shaderGroups );

    vk::DeviceSize rayGenOffset        = 0U * progSize; // Start at the beginning of _sbtBuffer
    vk::DeviceSize missOffset          = 1U * progSize; // Jump over raygen
    vk::DeviceSize chitGroupOffset     = 3U * progSize; // Jump over the previous two miss shaders
    vk::DeviceSize callableGroupOffset = 4U * progSize;

    vk::StridedBufferRegionKHR bufferRegionRayGen( _sbtBuffer.get( ), // buffer
                                                   rayGenOffset,      // offset
                                                   progSize,          // stride
                                                   sbtSize );         // size

    vk::StridedBufferRegionKHR bufferRegionMiss( _sbtBuffer.get( ), // buffer
                                                 missOffset,        // offset
                                                 progSize,          // stride
                                                 sbtSize );         // size

    vk::StridedBufferRegionKHR bufferRegionChit( _sbtBuffer.get( ), // buffer
                                                 chitGroupOffset,   // offset
                                                 progSize,          // stride
                                                 sbtSize );         // size

    vk::StridedBufferRegionKHR callableShaderBindingTable( _sbtBuffer.get( ),   // buffer
                                                           callableGroupOffset, // offset
                                                           progSize,            // stride
                                                           sbtSize );           // size

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

    // Copy ray tracing output (storage image) to swapchain image.
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
