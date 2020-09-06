#include "RayTracingBuilder.hpp"
#include "Components.hpp"
#include "Initializers.hpp"
#include "Helpers.hpp"
#include "Destructors.hpp"
#include "CommandBuffer.hpp"

namespace rx
{
  RayTracingBuilder::~RayTracingBuilder( )
  {
    destroy( );
  }

  void RayTracingBuilder::init( )
  {
    auto properties = g_physicalDevice.getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceRayTracingPropertiesKHR>( );
    m_rtProperties = properties.get<vk::PhysicalDeviceRayTracingPropertiesKHR>( );
  }

  void RayTracingBuilder::destroy( )
  {
    for ( Blas& blas : m_blas_ )
      blas.as.destroy( );

    m_tlas.as.destroy( );

    m_blas_.clear( );
  }

  Blas RayTracingBuilder::modelToBlas( const std::shared_ptr<Model> model ) const
  {
    vk::AccelerationStructureCreateGeometryTypeInfoKHR asCreate( vk::GeometryTypeKHR::eTriangles,       // geometryType
                                                                 model->m_indexBuffer.getCount( ) / 3,  // maxPrimitiveCount
                                                                 model->m_indexBuffer.getType( ),       // indexType
                                                                 model->m_vertexBuffer.getCount( ),     // maxVertexCount
                                                                 Vertex::getVertexFormat( ),            // vertexFormat
                                                                 VK_FALSE );                            // allowsTransforms

    vk::DeviceAddress vertexAddress = g_device.getBufferAddress( { model->m_vertexBuffer.get( ) } );
    vk::DeviceAddress indexAddress = g_device.getBufferAddress( { model->m_indexBuffer.get( ) } );

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

  vk::AccelerationStructureInstanceKHR RayTracingBuilder::instanceToVkGeometryInstanceKHR( const BlasInstance& instance )
  {
    Blas& blas { m_blas_[instance.blasId] };

    vk::AccelerationStructureDeviceAddressInfoKHR addressInfo( blas.as.as );
    vk::DeviceAddress blasAddress = g_device.getAccelerationStructureAddressKHR( addressInfo );

    glm::mat4 transpose = glm::transpose( instance.transform );

    vk::AccelerationStructureInstanceKHR gInst( { },                   // transform
                                                instance.instanceId,   // instanceCustomIndex
                                                instance.mask,         // mask
                                                instance.hitGroupId,   // instanceShaderBindingTableRecordOffset
                                                instance.flags,        // flags
                                                blasAddress );         // accelerationStructureReference

    memcpy( &gInst.transform, &transpose, sizeof( gInst.transform ) );

    return gInst;
  }

  void RayTracingBuilder::createBottomLevelAS( const std::vector<std::shared_ptr<Model>>& models )
  {
    // Clean up previous acceleration structures and free all memory.
    destroy( );

    // BLAS - Storing each primitive in a geometry.
    std::vector<Blas> allBlas;
    allBlas.reserve( models.size( ) );

    for ( const auto& obj : models )
    {
      Blas blas = modelToBlas( obj );

      // We could add more geometry in each BLAS, but we add only one for now.
      allBlas.emplace_back( blas );
    }

    buildBlas( allBlas, vk::BuildAccelerationStructureFlagBitsKHR::eAllowUpdate | vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastBuild );
  }

  void RayTracingBuilder::buildBlas( const std::vector<Blas>& blas_, vk::BuildAccelerationStructureFlagsKHR flags )
  {
    m_blas_ = blas_;

    vk::DeviceSize maxScratch = 0;

    // Is compaction requested?
    bool doCompaction = ( flags & vk::BuildAccelerationStructureFlagBitsKHR::eAllowCompaction ) == vk::BuildAccelerationStructureFlagBitsKHR::eAllowCompaction;

    RX_ASSERT( !doCompaction, "Compaction is not supported yet." );

    std::vector<vk::DeviceSize> originalSizes;
    originalSizes.resize( m_blas_.size( ) );

    // Iterate over the groups of geometries, creating one BLAS for each group
    int index = 0;
    for ( Blas& blas : m_blas_ )
    {
      vk::AccelerationStructureCreateInfoKHR asCreateInfo( { },                                                        // compactedSize
                                                           vk::AccelerationStructureTypeKHR::eBottomLevel,             // type
                                                           flags,                                                      // flags
                                                           static_cast<uint32_t>( blas.asCreateGeometryInfo.size( ) ), // maxGeometryCount
                                                           blas.asCreateGeometryInfo.data( ),                          // pGeometryInfos
                                                           { } );                                                      // deviceAddress

      blas.as = vk::Initializer::createAccelerationStructure( asCreateInfo );
      blas.flags = flags;

      vk::AccelerationStructureMemoryRequirementsInfoKHR memInfo( vk::AccelerationStructureMemoryRequirementsTypeKHR::eBuildScratch, // type
                                                                  vk::AccelerationStructureBuildTypeKHR::eDevice,                    // buildType
                                                                  blas.as.as );                                                      // accelerationStructure

      vk::MemoryRequirements2 memoryRequirements = rx::g_device.getAccelerationStructureMemoryRequirementsKHR( memInfo );
      vk::DeviceSize scratchSize = memoryRequirements.memoryRequirements.size;

      maxScratch = std::max( maxScratch, scratchSize );

      memInfo.type = vk::AccelerationStructureMemoryRequirementsTypeKHR::eObject;
      memoryRequirements = rx::g_device.getAccelerationStructureMemoryRequirementsKHR( memInfo );

      originalSizes[index] = memoryRequirements.memoryRequirements.size;
      
      ++index;
    }

    // Allocate the scratch buffers holding the temporary data of the acceleration structure builder.
    vk::MemoryAllocateFlagsInfo allocateFlags( vk::MemoryAllocateFlagBitsKHR::eDeviceAddress );

    Buffer scratchBuffer( maxScratch,                                                                              // size
                          vk::BufferUsageFlagBits::eRayTracingKHR | vk::BufferUsageFlagBits::eShaderDeviceAddress, // usage
                          { g_graphicsFamilyIndex },                                                               // queueFamilyIndices
                          vk::MemoryPropertyFlagBits::eDeviceLocal,                                                // memoryPropertyFlags
                          &allocateFlags );                                                                        // pNextMemory

    vk::BufferDeviceAddressInfo bufferInfo( scratchBuffer.get( ) );
    vk::DeviceAddress scratchAddress = g_device.getBufferAddress( bufferInfo );

    // Query size of compact BLAS.
    vk::UniqueQueryPool queryPool = vk::Initializer::createQueryPoolUnique( static_cast<uint32_t>( m_blas_.size( ) ), vk::QueryType::eAccelerationStructureCompactedSizeKHR );

    // Create a command buffer containing all the BLAS builds.
    vk::UniqueCommandPool commandPool = vk::Initializer::createCommandPoolUnique( { g_graphicsFamilyIndex } );
    int ctr = 0;

    CommandBuffer cmdBuf( commandPool.get( ), static_cast<uint32_t>( m_blas_.size( ) ) );

    index = 0;
    for ( Blas& blas : m_blas_ )
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

    cmdBuf.submitToQueue( g_graphicsQueue );
    
    // TODO: compaction. See: https://nvpro-samples.github.io/vk_raytracing_tutorial_KHR/  CTRL + F for: "The following is when compation flag is enabled. This part, which is optional, will ... "
  }

  void RayTracingBuilder::createTopLevelAS( const std::vector<std::shared_ptr<GeometryNode>>& nodes )
  {
    std::vector<BlasInstance> instances;
    instances.reserve( nodes.size( ) );

    for ( uint32_t i = 0; i < static_cast<uint32_t>( nodes.size( ) ); ++i )
    {
      BlasInstance rayInst;
      rayInst.transform = nodes[i]->m_worldTransform;
      rayInst.instanceId = i;
      rayInst.blasId = nodes[i]->m_rtInstance.m_modelIndex;
      rayInst.hitGroupId = 0; // We will use the same hit group for all objects
      rayInst.flags = vk::GeometryInstanceFlagBitsKHR::eTriangleFacingCullDisable;

      instances.push_back( rayInst );
    }
    
    buildTlas( instances, vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace | vk::BuildAccelerationStructureFlagBitsKHR::eAllowUpdate );
  }

  void RayTracingBuilder::buildTlas( const std::vector<BlasInstance>& instances, vk::BuildAccelerationStructureFlagsKHR flags )
  {
    m_tlas.flags = flags;

    vk::AccelerationStructureCreateGeometryTypeInfoKHR geometryCreate( vk::GeometryTypeKHR::eInstances,             // geometryType
                                                                       static_cast<uint32_t>( instances.size( ) ),  // maxPrimitiveCount
                                                                       { },                                         // indexType
                                                                       { },                                         // maxVertexCount
                                                                       { },                                         // vertexFormat
                                                                       VK_TRUE );                                   // allowsTransforms

    vk::AccelerationStructureCreateInfoKHR asCreateInfo( { },                                         // compactedSize
                                                         vk::AccelerationStructureTypeKHR::eTopLevel, // type
                                                         flags,                                       // flags
                                                         1,                                           // maxGeometryCount
                                                         &geometryCreate,                             // pGeometryInfos
                                                         { } );                                       // deviceAddress

    m_tlas.as = vk::Initializer::createAccelerationStructure( asCreateInfo );

    // TODO: Just like the BLAS creation we need a scratch buffer. We could save one allocation by re-using the BLAS scratch buffer.
    vk::AccelerationStructureMemoryRequirementsInfoKHR memoryRequirementsInfo( vk::AccelerationStructureMemoryRequirementsTypeKHR::eBuildScratch, // type
                                                                               vk::AccelerationStructureBuildTypeKHR::eDevice,                    // buildType
                                                                               m_tlas.as.as );                                                    // accelerationStructure

    vk::MemoryRequirements2 reqMem = g_device.getAccelerationStructureMemoryRequirementsKHR( memoryRequirementsInfo );
    vk::DeviceSize scratchSize = reqMem.memoryRequirements.size;

    // Allocate the scratch buffers holding the temporary data of the acceleration structure builder.
    vk::MemoryAllocateFlagsInfo allocateFlags( vk::MemoryAllocateFlagBitsKHR::eDeviceAddress );

    Buffer scratchBuffer( scratchSize,                                                                             // size
                          vk::BufferUsageFlagBits::eRayTracingKHR | vk::BufferUsageFlagBits::eShaderDeviceAddress, // usage
                          { g_graphicsFamilyIndex },                                                               // queueFamilyIndices
                          vk::MemoryPropertyFlagBits::eDeviceLocal,                                                // memoryPropertyFlags
                          &allocateFlags );                                                                        // pNextMemory

    vk::DeviceAddress scratchAddress = g_device.getBufferAddress( { scratchBuffer.get( ) } );

    std::vector<vk::AccelerationStructureInstanceKHR> geometryInstances;
    geometryInstances.reserve( instances.size( ) );

    for ( const auto& instance : instances )
    {
      geometryInstances.push_back( instanceToVkGeometryInstanceKHR( instance ) );
    }

    // Building the TLAS.
    vk::UniqueCommandPool commandPool = vk::Initializer::createCommandPoolUnique( g_graphicsFamilyIndex );
    CommandBuffer cmdBuf( commandPool.get( ) );

    // Create a buffer holding the actual instance data for use by the AS builder.
    vk::DeviceSize instanceDescsSizeInBytes = instances.size( ) * sizeof( vk::AccelerationStructureInstanceKHR );

    // Allocate the instance buffer and copy its contents from host to device memory.
    m_instanceBuffer.init( instanceDescsSizeInBytes,                                                                // size
                       vk::BufferUsageFlagBits::eRayTracingKHR | vk::BufferUsageFlagBits::eShaderDeviceAddress, // usage
                       { g_graphicsFamilyIndex },                                                               // queueFamilyIndices
                       vk::MemoryPropertyFlagBits::eHostVisible,                                                // memoryPropertyFlags
                       &allocateFlags );                                                                        // pNextMemory

    m_instanceBuffer.fill<vk::AccelerationStructureInstanceKHR>( geometryInstances.data( ) );

    vk::DeviceAddress instanceAddress = g_device.getBufferAddress( { m_instanceBuffer.get( ) } );

    // Make sure the copy of the instance buffer are copied before triggering the acceleration structure build.
    vk::MemoryBarrier barrier( vk::AccessFlagBits::eTransferWrite,                   // srcAccessMask
                               vk::AccessFlagBits::eAccelerationStructureWriteKHR ); // dstAccessMask

    cmdBuf.begin( 0 );

    cmdBuf.get( 0 ).pipelineBarrier( vk::PipelineStageFlagBits::eTransfer,                            // srcStageMask
                                     vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR,       // dstStageMask
                                     { },                                                             // dependencyFlags
                                     1,                                                               // memoryBarrierCount
                                     &barrier,                                                        // pMemoryBarriers
                                     0,                                                               // bufferMemoryBarrierCount
                                     nullptr,                                                         // pBufferMemoryBarriers
                                     0,                                                               // imageMemoryBarrierCount 
                                     nullptr );                                                       // pImageMemoryBarriers
  
    // Build the TLAS.
    vk::AccelerationStructureGeometryDataKHR geometry;
    geometry.instances.arrayOfPointers = VK_FALSE;
    geometry.instances.data.deviceAddress = instanceAddress;

    vk::AccelerationStructureGeometryKHR topAsGeometry( vk::GeometryTypeKHR::eInstances, // geometryType
                                                        geometry,                        // geometry
                                                        { } );                           // flags
  
    const vk::AccelerationStructureGeometryKHR* pGeometry = &topAsGeometry;

    vk::AccelerationStructureBuildGeometryInfoKHR topAsInfo( vk::AccelerationStructureTypeKHR::eTopLevel,      // type
                                                             flags,                                            // flags
                                                             VK_FALSE,                                         // update
                                                             nullptr,                                          // srcAccelerationStructure
                                                             m_tlas.as.as,                                     // dstAccelerationStructure
                                                             VK_FALSE,                                         // geometryArrayOfPointers
                                                             1,                                                // geometryCount
                                                             &pGeometry,                                       // ppGeometries
                                                             scratchAddress );                                 // scratchData

    // Build Offsets info: n instances.
    vk::AccelerationStructureBuildOffsetInfoKHR buildOffsetInfo( static_cast<uint32_t>( instances.size( ) ), // primitiveCount
                                                                 0,                                          // primitiveOffset
                                                                 0,                                          // firstVertex
                                                                 0 );                                        // transformOffset

    const vk::AccelerationStructureBuildOffsetInfoKHR* pBuildOffsetInfo = &buildOffsetInfo;

    // Build the TLAS.
    cmdBuf.get( 0 ).buildAccelerationStructureKHR( 1, &topAsInfo, &pBuildOffsetInfo );

    cmdBuf.end( 0 );
    cmdBuf.submitToQueue( g_graphicsQueue );
  }

  void RayTracingBuilder::createStorageImage( vk::Extent2D swapchainExtent )
  {
    auto storageImageInfo = vk::Helper::getImageCreateInfo( vk::Extent3D( swapchainExtent.width, swapchainExtent.height, 1 ) );
    storageImageInfo.usage = vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eStorage;
    storageImageInfo.format = vk::Format::eB8G8R8A8Unorm; // TODO: make this the surface format, and not hard-coded

    m_storageImage.init( storageImageInfo );
    m_storageImage.transitionToLayout( vk::ImageLayout::eGeneral );

    m_storageImageView = vk::Initializer::createImageViewUnique( m_storageImage.get( ), m_storageImage.getFormat( ) );
  }

  void RayTracingBuilder::createShaderBindingTable( vk::Pipeline rtPipeline )
  {
    uint32_t groupCount = g_shaderGroups;
    uint32_t groupHandleSize = m_rtProperties.shaderGroupHandleSize;
    uint32_t baseAlignment = m_rtProperties.shaderGroupBaseAlignment;

    uint32_t sbtSize = groupCount * baseAlignment;

    m_sbtBuffer.init( sbtSize,
                      vk::BufferUsageFlagBits::eTransferSrc,
                      { g_graphicsFamilyIndex },
                      vk::MemoryPropertyFlagBits::eHostVisible );

    std::vector<uint8_t> shaderHandleStorage( sbtSize );
    g_device.getRayTracingShaderGroupHandlesKHR( rtPipeline,
                                                 0,
                                                 groupCount,
                                                 sbtSize,
                                                 shaderHandleStorage.data( ) );

    void* mapped = NULL;
    g_device.mapMemory( m_sbtBuffer.getMemory( ), 0, m_sbtBuffer.getSize( ), { }, &mapped );

    auto* pData = reinterpret_cast<uint8_t*>( mapped );
    for ( uint32_t i = 0; i < g_shaderGroups; ++i )
    {
      memcpy( pData, shaderHandleStorage.data( ) + i * groupHandleSize, groupHandleSize );  // raygen
      pData += baseAlignment;
    }

    g_device.unmapMemory( m_sbtBuffer.getMemory( ) );
  }

  void RayTracingBuilder::rayTrace( vk::CommandBuffer swapchainCommandBuffer, vk::Image swapchainImage, vk::Extent2D extent )
  {
    vk::DeviceSize progSize = m_rtProperties.shaderGroupBaseAlignment;
    vk::DeviceSize sbtSize = progSize * static_cast<vk::DeviceSize>( g_shaderGroups );

    vk::DeviceSize rayGenOffset = RX_SHADER_GROUP_INDEX_RGEN * progSize;  // Start at the beginning of m_sbtBuffer
    vk::DeviceSize missOffset = RX_SHADER_GROUP_INDEX_MISS * progSize;  // Jump over raygen
    vk::DeviceSize missStride = progSize;
    vk::DeviceSize chitGroupOffset = RX_SHADER_GROUP_INDEX_CHIT * progSize;  // Jump over the previous shaders
    vk::DeviceSize chitGroupStride = progSize;

    vk::StridedBufferRegionKHR bufferRegionRayGen( m_sbtBuffer.get( ), // buffer
                                                   rayGenOffset,       // offset
                                                   progSize,           // stride
                                                   sbtSize );          // size

    vk::StridedBufferRegionKHR bufferRegionMiss( m_sbtBuffer.get( ), // buffer
                                                 missOffset,         // offset
                                                 progSize,           // stride
                                                 sbtSize );          // size

    vk::StridedBufferRegionKHR bufferRegionChit( m_sbtBuffer.get( ), // buffer
                                                 chitGroupOffset,    // offset
                                                 progSize,           // stride
                                                 sbtSize );          // size           

    vk::StridedBufferRegionKHR callableShaderBindingTable;

    swapchainCommandBuffer.traceRaysKHR( &bufferRegionRayGen,         // pRaygenShaderBindingTable
                                         &bufferRegionMiss,           // pMissShaderBindingTable
                                         &bufferRegionChit,           // pHitShaderBindingTable
                                         &callableShaderBindingTable, // pCallableShaderBindingTable
                                         extent.width,                // width
                                         extent.height,               // height
                                         1 );                         // depth
 
    // Image layout transitions for copying.
    vk::Helper::transitionImageLayout( swapchainImage, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, swapchainCommandBuffer );
    m_storageImage.transitionToLayout( vk::ImageLayout::eTransferSrcOptimal, swapchainCommandBuffer );

    // Copy ray tracing output (storage image) to swapchain image.
    vk::ImageCopy copyRegion( { vk::ImageAspectFlagBits::eColor, 0, 0, 1 }, // srcSubresource
                              { 0, 0, 0 },                                  // srcOffset
                              { vk::ImageAspectFlagBits::eColor, 0, 0, 1 }, // dstSubresource
                              { 0, 0, 0 },                                  // dstOffset
                              { extent.width, extent.height, 1 } );         // extent

    swapchainCommandBuffer.copyImage( m_storageImage.get( ),                // srcImage 
                                      vk::ImageLayout::eTransferSrcOptimal, // srcImageLayout 
                                      swapchainImage,                       // dstImage
                                      vk::ImageLayout::eTransferDstOptimal, // dstImageLayout
                                      1,                                    // regionCount
                                      &copyRegion );                        // pRegions

    // Undo image layout transitions.
    vk::Helper::transitionImageLayout( swapchainImage, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::ePresentSrcKHR, swapchainCommandBuffer ); // TODO: Check, might cause crash when GUI is activated.
    m_storageImage.transitionToLayout( vk::ImageLayout::eGeneral, swapchainCommandBuffer );
  }
}