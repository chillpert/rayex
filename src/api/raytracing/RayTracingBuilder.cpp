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

  Blas RayTracingBuilder::objectToVkGeometryKHR( const std::shared_ptr<Model> model ) const
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

  vk::AccelerationStructureInstanceKHR RayTracingBuilder::instanceToVkGeometryInstanceKHR( const BlasInstance& instance ) const
  {
    vk::DeviceAddress blasAddress = g_device.getAccelerationStructureAddressKHR( { m_blas_[instance.blasId].as.as }, *g_dispatchLoaderDynamic );

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
      Blas blas = objectToVkGeometryKHR( obj );

      // We could add more geometry in each BLAS, but we add only one for now.
      allBlas.emplace_back( blas );
    }

    buildBlas( allBlas );
  }

  void RayTracingBuilder::buildBlas( const std::vector<Blas>& blas_, vk::BuildAccelerationStructureFlagsKHR flags )
  {
    m_blas_ = blas_;

    vk::DeviceSize maxScratch( 0 );

    // Is compaction requested?
    bool doCompaction = ( flags & vk::BuildAccelerationStructureFlagBitsKHR::eAllowCompaction ) == vk::BuildAccelerationStructureFlagBitsKHR::eAllowCompaction;

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

      vk::AccelerationStructureMemoryRequirementsInfoKHR memInfo( vk::AccelerationStructureMemoryRequirementsTypeKHR::eObject, // type
                                                                  vk::AccelerationStructureBuildTypeKHR::eDevice,              // buildType
                                                                  blas.as.as );                                                // accelerationStructure

      vk::MemoryRequirements2 memoryRequirements = rx::g_device.getAccelerationStructureMemoryRequirementsKHR( memInfo, *rx::g_dispatchLoaderDynamic );
      vk::DeviceSize scratchSize = memoryRequirements.memoryRequirements.size;

      maxScratch = std::max( maxScratch, scratchSize );

      memInfo.type = vk::AccelerationStructureMemoryRequirementsTypeKHR::eObject;
      memoryRequirements = rx::g_device.getAccelerationStructureMemoryRequirementsKHR( memInfo, *rx::g_dispatchLoaderDynamic );

      originalSizes[index] = memoryRequirements.memoryRequirements.size;
    }

    // Allocate the scratch buffers holding the temporary data of the acceleration structure builder.
    vk::MemoryAllocateFlagsInfo allocateFlags( vk::MemoryAllocateFlagBitsKHR::eDeviceAddress );

    Buffer scratchBuffer( maxScratch,                                                                              // size
                          vk::BufferUsageFlagBits::eRayTracingKHR | vk::BufferUsageFlagBits::eShaderDeviceAddress, // usage
                          { g_graphicsFamilyIndex },                                                               // queueFamilyIndices
                          vk::MemoryPropertyFlagBits::eDeviceLocal,                                                // memoryPropertyFlags
                          &allocateFlags );                                                                        // pNextMemory

    vk::DeviceAddress scratchAddress = g_device.getBufferAddress( { scratchBuffer.get( ) } );

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
      cmdBuf.get( index ).buildAccelerationStructureKHR( 1, &bottomAsInfo, pBuildOffset.data( ), *g_dispatchLoaderDynamic );


      // Since the scratch buffer is reused across builds, we need a barrier to ensure one build is finished before starting the next one.
      vk::MemoryBarrier barrier( vk::AccessFlagBits::eAccelerationStructureWriteKHR,  // srcAccessMask
                                 vk::AccessFlagBits::eAccelerationStructureReadKHR ); // dstAccessMask
      

      cmdBuf.get( index ).pipelineBarrier( vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR,   // srcStageMask
                                       vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR,       // dstStageMask
                                       { },                                                             // dependencyFlags
                                       1,                                                               // memoryBarrierCount
                                       &barrier,                                                        // pMemoryBarriers
                                       0,                                                               // bufferMemoryBarrierCount
                                       nullptr,                                                         // pBufferMemoryBarriers
                                       0,                                                               // imageMemoryBarrierCount 
                                       nullptr );                                                       // pImageMemoryBarriers

      if ( doCompaction )
      {
        cmdBuf.get( index ).writeAccelerationStructuresPropertiesKHR( 1,                                                     // accelerationStructureCount
                                                                      &blas.as.as,                                           // pAccelerationStructures
                                                                      vk::QueryType::eAccelerationStructureCompactedSizeKHR, // queryType
                                                                      queryPool.get( ),                                      // queryPool
                                                                      ctr++,                                                 // firstQuery
                                                                      *g_dispatchLoaderDynamic );                            // dispatch
      }

      cmdBuf.end( index );

      ++index;
    }

    cmdBuf.submitToQueue( g_graphicsQueue );
     
    // TODO: compaction. See: https://nvpro-samples.github.io/vk_raytracing_tutorial_KHR/  CTRL + F for: "The following is when compation flag is enabled. This part, which is optional, will ... "
  }

  void RayTracingBuilder::createTopLevelAS( const std::vector<std::shared_ptr<GeometryNodeBase>>& nodes )
  {
    std::vector<BlasInstance> instances;
    instances.reserve( nodes.size( ) );

    for ( uint32_t i = 0; i < static_cast<uint32_t>( nodes.size( ) ); ++i )
    {
      BlasInstance rayInst;
      rayInst.transform = nodes[i]->m_worldTransform;
      rayInst.instanceId = i;
      rayInst.blasId = nodes[i]->m_rtInstance.m_objIndex;
      rayInst.hitGroupId = 0; // We will use the same hit group for all objects
      rayInst.flags = vk::GeometryInstanceFlagBitsKHR::eTriangleFacingCullDisable;

      instances.push_back( rayInst );
    }
    
    buildTlas( instances, vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace );
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

    vk::MemoryRequirements2 reqMem = g_device.getAccelerationStructureMemoryRequirementsKHR( memoryRequirementsInfo, *g_dispatchLoaderDynamic );
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
    m_instBuffer.init( instanceDescsSizeInBytes,                                                                // size
                       vk::BufferUsageFlagBits::eRayTracingKHR | vk::BufferUsageFlagBits::eShaderDeviceAddress, // usage
                       { g_graphicsFamilyIndex },                                                               // queueFamilyIndices
                       vk::MemoryPropertyFlagBits::eDeviceLocal,                                                // memoryPropertyFlags
                       &allocateFlags );                                                                        // pNextMemory


    vk::DeviceAddress instanceAddress = g_device.getBufferAddress( { m_instBuffer.get( ) } );

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
    cmdBuf.get( 0 ).buildAccelerationStructureKHR( 1, &topAsInfo, &pBuildOffsetInfo, *g_dispatchLoaderDynamic );

    cmdBuf.end( 0 );
    cmdBuf.submitToQueue( g_graphicsQueue );
  }

  void RayTracingBuilder::createDescriptorSet( )
  {
    // TLAS.
    vk::DescriptorSetLayoutBinding tlasBinding( 0,                                                                             // binding
                                                vk::DescriptorType::eAccelerationStructureKHR,                                 // descriptorType
                                                1,                                                                             // descriptorCount
                                                vk::ShaderStageFlagBits::eRaygenKHR | vk::ShaderStageFlagBits::eClosestHitKHR, // stageFlags
                                                nullptr );                                                                     // pImmutableSamplers

    // Output image.
    vk::DescriptorSetLayoutBinding outputImageBinding( 1,                                   // binding
                                                       vk::DescriptorType::eStorageImage,   // descriptorType
                                                       1,                                   // descriptorCount
                                                       vk::ShaderStageFlagBits::eRaygenKHR, // stageFlags
                                                       nullptr );                           // pImmutableSamplers

    m_descriptorSetLayout.addBinding( tlasBinding );
    m_descriptorSetLayout.addBinding( outputImageBinding );

    m_descriptorSetLayout.init( );

    m_descriptorPool = vk::Initializer::createDescriptorPoolUnique( vk::Helper::getPoolSizes( { tlasBinding, outputImageBinding } ), // poolSizes
                                                                    g_swapchainImageCount );                                         // maxSets

    m_descriptorSets.init( m_descriptorPool.get( ), g_swapchainImageCount, std::vector<vk::DescriptorSetLayout>( g_swapchainImageCount, m_descriptorSetLayout.get( ) ) );

    vk::WriteDescriptorSetAccelerationStructureKHR descAsInfo;
  }
}