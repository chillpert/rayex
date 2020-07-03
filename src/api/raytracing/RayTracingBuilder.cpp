#include "RayTracingBuilder.hpp"
#include "Components.hpp"
#include "Initializers.hpp"
#include "Helpers.hpp"
#include "Destructors.hpp"

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
                                                                  blas.as.as );                                              // accelerationStructure

      vk::MemoryRequirements2 memoryRequirements = rx::g_device.getAccelerationStructureMemoryRequirementsKHR( memInfo, *rx::g_dispatchLoaderDynamic );
      vk::DeviceSize scratchSize = memoryRequirements.memoryRequirements.size;

      maxScratch = std::max( maxScratch, scratchSize );

      memInfo.type = vk::AccelerationStructureMemoryRequirementsTypeKHR::eObject;
      memoryRequirements = rx::g_device.getAccelerationStructureMemoryRequirementsKHR( memInfo, *rx::g_dispatchLoaderDynamic );

      originalSizes[index] = memoryRequirements.memoryRequirements.size;
    }

    // Allocate the scratch buffers holding the temporary data of the acceleration structure builder.
    /*
    Buffer scratchBuffer( maxScratch,                                                                              // size
                          vk::BufferUsageFlagBits::eRayTracingKHR | vk::BufferUsageFlagBits::eShaderDeviceAddress, // usage
                          { g_graphicsFamilyIndex } );                                                             // queueFamilyIndices
                          
    */

  }
}