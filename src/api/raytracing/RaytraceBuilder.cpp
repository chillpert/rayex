#include "RaytraceBuilder.hpp"
#include "CommandBuffer.hpp"
#include "Buffer.hpp"
#include "Components.hpp"
#include "Initializers.hpp"
#include "Helpers.hpp"
#include "Destructors.hpp"
#include "Components.hpp"

namespace rx
{
  void RaytraceBuilder::init( )
  {
    auto properties = g_physicalDevice.getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceRayTracingPropertiesKHR>( );
    m_rayTracingProperties = properties.get<vk::PhysicalDeviceRayTracingPropertiesKHR>( );
  }

  void RaytraceBuilder::destroy( )
  {
    m_tlas.destroy( );
    for ( auto& blas : m_blas )
      blas.destroy( );

    //vk::Destructor::destroyImageView( m_storageImageView );

    //m_storageImage.destroy();

    m_descriptorSet.free( );
    m_descriptorSetLayout.destroy( );
    //m_descriptorPool.destroy();
  }

  void RaytraceBuilder::initAccelerationStructures( const std::vector<std::shared_ptr<GeometryNodeBase>>& nodes, const std::vector<std::shared_ptr<Model>>& models, const Surface* const surface )
  {
    // Create all bottom level acceleration structures at once.
    initBottomLevelAS_( vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace, models, m_blas );

    // Create a single bottom level acceleration structure.
    m_tlas.init( nodes, vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace );

    // Buffers
    std::vector<Buffer> buffers( nodes.size( ) );

    struct RaytraceInstanceData
    {
      RaytracingInstance instance;
      glm::mat4 worldTransform;
    };

    vk::AccelerationStructureMemoryRequirementsInfoKHR memReqInfo{
      vk::AccelerationStructureMemoryRequirementsTypeKHR::eBuildScratch, // type
      { }, // buildType
      nullptr // accelerationStructure
    };

    vk::DeviceSize blasTotalMemorySize = 0;

    for ( size_t i = 0; i < buffers.size( ); ++i )
    {
      buffers[i].init( sizeof( RaytraceInstanceData ),
                       vk::BufferUsageFlagBits::eRayTracingKHR,
                       { },
                       vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );

      RaytraceInstanceData data{
        .instance = nodes[i]->m_rtInstance,
        .worldTransform = nodes[i]->m_worldTransform
      };

      buffers[i].fill( &data );

      memReqInfo.accelerationStructure = m_blas[i].get( );


      vk::MemoryRequirements2 memReqBlas = g_device.getAccelerationStructureMemoryRequirementsKHR( memReqInfo, *g_dispatchLoaderDynamic );

      blasTotalMemorySize += memReqBlas.memoryRequirements.size;
    }

    vk::MemoryRequirements2 memReqTlas = g_device.getAccelerationStructureMemoryRequirementsKHR( memReqInfo, *g_dispatchLoaderDynamic );
    const vk::DeviceSize scratchBufferSize = std::max( blasTotalMemorySize, memReqTlas.memoryRequirements.size );

    Buffer scratchBuffer( scratchBufferSize,
                          vk::BufferUsageFlagBits::eRayTracingKHR,
                          { },
                          vk::MemoryPropertyFlagBits::eDeviceLocal );

    // Create descriptor set.
    initDescriptorSet( surface );
  }

  void RaytraceBuilder::initDescriptorSet( const Surface* const surface )
  {
    std::vector<vk::DescriptorPoolSize> poolSizes =
    {
      { vk::DescriptorType::eAccelerationStructureKHR, 1 },
      { vk::DescriptorType::eStorageImage, 1 },
      { vk::DescriptorType::eUniformBuffer, 1 },
      { vk::DescriptorType::eStorageBuffer, 2 }
    };

    static bool firstRun = true;
    if ( firstRun )
    {
      // Init descriptor pool.
      m_descriptorPool = vk::Initializer::createDescriptorPoolUnique( poolSizes, 1, vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet );

      firstRun = false;
    }

    // Create raytracing shaders.
    m_rayGen = vk::Initializer::createShaderModuleUnique( RX_SHADER_PATH "raygen.rgen" );
    m_miss = vk::Initializer::createShaderModuleUnique( RX_SHADER_PATH "miss.rmiss" );
    m_closestHit = vk::Initializer::createShaderModuleUnique( RX_SHADER_PATH "closesthit.rchit" );

    // Init descriptor set layout.
    vk::DescriptorSetLayoutBinding asLayoutBinding{
      0,                                                                              // binding
      vk::DescriptorType::eAccelerationStructureKHR,                                  // descriptorType
      1,                                                                              // descriptorCount
      vk::ShaderStageFlagBits::eRaygenKHR | vk::ShaderStageFlagBits::eClosestHitKHR,  // stageFlags
      nullptr                                                                         // pImmutableSamplers
    };

    vk::DescriptorSetLayoutBinding storageImageLayoutBinding{
      1,                                    // binding
      vk::DescriptorType::eStorageImage,    // descriptorType
      1,                                    // descriptorCount
      vk::ShaderStageFlagBits::eRaygenKHR,  // stageFlags
      nullptr                               // pImmutableSamplers
    };

    m_descriptorSetLayout.addBinding( asLayoutBinding );
    m_descriptorSetLayout.addBinding( storageImageLayoutBinding );

    m_descriptorSetLayout.init( );

    // Create the descriptor set.
    m_descriptorSet.init( m_descriptorPool.get( ), 1, { m_descriptorSetLayout.get( ) } );

    // Create the storage image.
    auto imageCreateInfo = vk::Helper::getImageCreateInfo( vk::Extent3D( surface->getCapabilities( ).currentExtent, 1 ) );
    imageCreateInfo.format = surface->getFormat( );
    imageCreateInfo.usage = vk::ImageUsageFlagBits::eStorage;

    m_storageImage.init( imageCreateInfo );

    m_storageImageView = vk::Initializer::createImageViewUnique( m_storageImage.get( ), m_storageImage.getFormat( ) );

    // Update descriptor set.
    m_descriptorSet.update( m_tlas.get( ), m_storageImageView.get( ) );
  }
}