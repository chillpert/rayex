#include "api/descriptors/Descriptor.hpp"
#include "api/utility/Helpers.hpp"
#include "api/utility/Initializers.hpp"
#include "api/misc/Components.hpp"

namespace RAYEXEC_NAMESPACE
{
  DescriptorManager::~DescriptorManager( )
  {
    destroy( );
  }

  void DescriptorManager::addDescriptor( const std::any& descriptor, uint32_t binding, vk::DescriptorType descriptorType, vk::ShaderStageFlags shaderStage, vk::DeviceSize size )
  {
    if ( this->initialized )
      RX_ERROR( "Can not add descriptor if descriptor sets were already initialized and updated." );

    this->bindings.push_back( vk::Helper::getDescriptorSetLayoutBinding( binding, descriptorType, shaderStage ) );
    this->descriptors.push_back( descriptor );
    this->sizes.push_back( size );
  }

  void DescriptorManager::init( uint32_t maxSets )
  {
    // Init descriptor set layout.
    this->layout = vk::Initializer::initDescriptorSetLayout( this->bindings );

    // One layout per swapchain image.
    this->layouts = { g_swapchainImageCount, this->layout};

    // Init descriptor pool.
    this->pool = vk::Initializer::initDescriptorPool( vk::Helper::getPoolSizes( this->bindings ), maxSets );

    // Init descriptor sets.
    this->sets = vk::Initializer::initDescriptorSets( this->pool, this->layout );
  }

  void DescriptorManager::update( )
  {
    // Make sure descriptors and bindings habe the same size

    std::vector<vk::WriteDescriptorSet> descriptorWrites;
    descriptorWrites.reserve( this->bindings.size( ) );

    for ( size_t i = 0; i < this->layouts.size( ); ++i )
    {
      for ( size_t j = 0; j < this->bindings.size( ); ++j )
      {
        switch ( this->bindings[j].descriptorType )
        {
          case vk::DescriptorType::eUniformBuffer:
          {

            auto ubo = std::any_cast<std::vector<vk::Buffer>>( descriptors[j] );

            vk::DescriptorBufferInfo uboInfo( ubo[i],     // buffer
                                              0,          // offset
                                              sizes[j] ); // range

            vk::WriteDescriptorSet write( this->sets[i],                      // dstSet
                                          this->bindings[j].binding,          // dstBinding
                                          0,                                  // dstArrayElement
                                          1,                                  // descriptorCount
                                          this->bindings[j].descriptorType, // descriptorType
                                          nullptr,                            // pImageInfo
                                          &uboInfo,                           // pBufferInfo
                                          nullptr );                          // pTextelBufferView

            descriptorWrites.push_back( write );

            break;
          }
          case vk::DescriptorType::eStorageBuffer:
          {

            auto ubo = std::any_cast<vk::Buffer>( descriptors[j] );

            vk::DescriptorBufferInfo info( ubo,        // buffer
                                           0,          // offset
                                           sizes[j] ); // range

            vk::WriteDescriptorSet write( this->sets[i],                      // dstSet
                                          this->bindings[j].binding,          // dstBinding
                                          0,                                  // dstArrayElement
                                          1,                                  // descriptorCount
                                          this->bindings[j].descriptorType,   // descriptorType
                                          nullptr,                            // pImageInfo
                                          &info,                           // pBufferInfo
                                          nullptr );                          // pTextelBufferView

            descriptorWrites.push_back( write );

            break;
          }
          default:
            RX_ERROR( "NOT HANDLED" );
            break;
        }
      }

      g_device.updateDescriptorSets( descriptorWrites, 0 );
    }
  }

  void DescriptorManager::destroy( )
  {
    if ( this->layout )
    {
      g_device.destroyDescriptorSetLayout( this->layout );
      this->layout = nullptr;
    }

    if ( this->pool )
    {
      g_device.destroyDescriptorPool( this->pool );
      this->pool = nullptr;
    }

    this->bindings.clear( );
  }
}