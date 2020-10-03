#include "api/Bindings.hpp"

namespace RAYEXEC_NAMESPACE
{
  void Bindings::add( uint32_t binding, vk::DescriptorType type, vk::ShaderStageFlags stage, uint32_t count, vk::DescriptorBindingFlags flags )
  {
    vk::DescriptorSetLayoutBinding temp( binding,   // binding
                                         type,      // descriptorType
                                         count,     // descriptorCount
                                         stage,     // stageFlags
                                         nullptr ); // pImmutableSamplers

    this->bindings.push_back( temp );
    this->flags.push_back( flags );

    // Resources are created per swapchain image. This operation will only be executed once.
    if ( this->writes.size( ) != g_swapchainImageCount )
    {
      this->writes.resize( g_swapchainImageCount );
    }

    // For every new binding that is added, the size of each write in writes will get increased by one.
    for ( auto& write : this->writes )
    {
      write.resize( write.size( ) + 1 );
    }
  }

  auto Bindings::initLayoutUnique( ) -> vk::UniqueDescriptorSetLayout
  {
    auto bindingCount = static_cast<uint32_t>( this->bindings.size( ) );
  
    vk::DescriptorSetLayoutCreateInfo createInfo( { },                      // flags
                                                  bindingCount,             // bindingCount
                                                  this->bindings.data( ) ); // pBindings

    vk::DescriptorSetLayoutBindingFlagsCreateInfoEXT layoutFlags( bindingCount,          // bindingCount
                                                                  this->flags.data( ) ); // pBindingFlags
    createInfo.pNext = &layoutFlags;

    auto layout = g_device.createDescriptorSetLayoutUnique( createInfo );
    RX_ASSERT( layout, "Failed to create descriptor set layout." );

    return std::move( layout );
  }

  auto Bindings::initPoolUnique( uint32_t maxSets, vk::DescriptorPoolCreateFlags flags ) -> vk::UniqueDescriptorPool
  {
    std::vector<vk::DescriptorPoolSize> tPoolSizes;

    if ( this->poolSizes.has_value( ) )
    {
      tPoolSizes = this->poolSizes.value( );
    }
    else
    {
      tPoolSizes = vk::Helper::getPoolSizes( this->bindings, maxSets );
    }

    for ( auto flag : this->flags )
    {
      if ( flag == vk::DescriptorBindingFlagBits::eUpdateAfterBind )
      {
        flags |= vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind;
        break;
      }
    }

    vk::DescriptorPoolCreateInfo createInfo( flags,                                       // flags
                                             maxSets,                                     // maxSets
                                             static_cast<uint32_t>( tPoolSizes.size( ) ), // poolSizeCount
                                             tPoolSizes.data( ) );                        // pPoolSizes

    auto pool = g_device.createDescriptorPoolUnique( createInfo );
    RX_ASSERT( pool, "Failed to create descriptor pool." );

    return std::move( pool );
  }

  void Bindings::update( )
  {
    for ( const auto& write : this->writes )
    {
      g_device.updateDescriptorSets( static_cast<uint32_t>( write.size( ) ), write.data( ), 0, nullptr );
    }
  }

  auto Bindings::write( vk::DescriptorSet set, size_t writesIndex, uint32_t binding, uint32_t arrayElement ) -> size_t
  {
    vk::WriteDescriptorSet result;

    for ( size_t i = 0; i < this->bindings.size( ); ++i )
    {
      if ( this->bindings[i].binding == binding )
      {
        result.descriptorCount = 1;
        result.descriptorType  = this->bindings[i].descriptorType;
        result.dstBinding      = binding;
        result.dstSet          = set;
        result.dstArrayElement = arrayElement;

        this->writes[writesIndex][i] = result;
        return i;
      }
    }

    RX_ASSERT( false, "Failed to write binding to set. Binding could not be found." );
    return 0;
  }

  void Bindings::write( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, vk::WriteDescriptorSetAccelerationStructureKHR* pWriteDescriptorSetAccelerationStructureKHR )
  {
    for ( size_t i = 0; i < sets.size( ); ++i )
    {
      size_t j                 = write( sets[i], i, binding );
      this->writes[i][j].pNext = pWriteDescriptorSetAccelerationStructureKHR;
    }
  }

  void Bindings::write( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, vk::DescriptorImageInfo* pImageInfo )
  {
    for ( size_t i = 0; i < sets.size( ); ++i )
    {
      size_t j                      = write( sets[i], i, binding );
      this->writes[i][j].pImageInfo = pImageInfo;
    }
  }

  void Bindings::write( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, vk::DescriptorBufferInfo* pBufferInfo )
  {
    for ( size_t i = 0; i < sets.size( ); ++i )
    {
      size_t j                       = write( sets[i], i, binding );
      this->writes[i][j].pBufferInfo = pBufferInfo;
    }
  }

  void Bindings::write( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, const std::vector<vk::DescriptorBufferInfo>& uniformBufferInfos )
  {
    RX_ASSERT( uniformBufferInfos.size( ) == sets.size( ), "Failed to write descriptor for uniform buffer because there are not as many uniform buffers as there are swapchain images." );

    for ( size_t i = 0; i < sets.size( ); ++i )
    {
      size_t j                       = write( sets[i], i, binding );
      this->writes[i][j].pBufferInfo = &uniformBufferInfos[i];
    }
  }

  void Bindings::setPoolSizes( const std::vector<vk::DescriptorPoolSize>& poolSizes )
  {
    this->poolSizes = poolSizes;
  }
} // namespace RAYEXEC_NAMESPACE