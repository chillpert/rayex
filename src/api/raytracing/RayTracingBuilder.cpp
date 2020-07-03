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

    m_blas_.clear( );
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
      Blas blas = vk::Helper::objectToVkGeometryKHR( obj );

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
}