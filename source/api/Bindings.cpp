#include "api/Bindings.hpp"

namespace RAYEX_NAMESPACE
{
  void Bindings::add( uint32_t binding, vk::DescriptorType type, vk::ShaderStageFlags stage, uint32_t count, vk::DescriptorBindingFlags flags, vk::Sampler* pImmutableSamplers )
  {
    vk::DescriptorSetLayoutBinding temp( binding,              // binding
                                         type,                 // descriptorType
                                         count,                // descriptorCount
                                         stage,                // stageFlags
                                         pImmutableSamplers ); // pImmutableSamplers

    _bindings.push_back( temp );
    _flags.push_back( flags );

    // Resources are created per swapchain image. This operation will only be executed once.
    if ( _writes.size( ) != components::maxResources )
    {
      _writes.resize( components::maxResources );
    }

    // For every new binding that is added, the size of each write in writes will get increased by one.
    for ( auto& write : _writes )
    {
      write.resize( write.size( ) + 1 );
    }
  }

  auto Bindings::initLayoutUnique( vk::DescriptorSetLayoutCreateFlags flags ) -> vk::UniqueDescriptorSetLayout
  {
    auto bindingCount = static_cast<uint32_t>( _bindings.size( ) );

    vk::DescriptorSetLayoutCreateInfo createInfo( flags,               // flags
                                                  bindingCount,        // bindingCount
                                                  _bindings.data( ) ); // pBindings

    vk::DescriptorSetLayoutBindingFlagsCreateInfoEXT layoutFlags( bindingCount,     // bindingCount
                                                                  _flags.data( ) ); // pBindingFlags
    createInfo.pNext = &layoutFlags;

    auto layout = components::device.createDescriptorSetLayoutUnique( createInfo );
    RX_ASSERT( layout.get( ), "Failed to create descriptor set layout." );

    return std::move( layout );
  }

  auto Bindings::initPoolUnique( uint32_t maxSets, vk::DescriptorPoolCreateFlags flags ) -> vk::UniqueDescriptorPool
  {
    std::vector<vk::DescriptorPoolSize> tPoolSizes;

    if ( _poolSizes.has_value( ) )
    {
      tPoolSizes = _poolSizes.value( );
    }
    else
    {
      tPoolSizes = vk::Helper::getPoolSizes( _bindings, maxSets );
    }

    for ( auto flag : _flags )
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

    auto pool = components::device.createDescriptorPoolUnique( createInfo );
    RX_ASSERT( pool.get( ), "Failed to create descriptor pool." );

    return std::move( pool );
  }

  void Bindings::update( )
  {
    for ( const auto& write : _writes )
    {
      components::device.updateDescriptorSets( static_cast<uint32_t>( write.size( ) ), write.data( ), 0, nullptr );
    }
  }

  auto Bindings::write( vk::DescriptorSet set, size_t writeIndex, uint32_t binding ) -> size_t
  {
    vk::WriteDescriptorSet result;

    for ( size_t i = 0; i < _bindings.size( ); ++i )
    {
      if ( _bindings[i].binding == binding )
      {
        result.descriptorCount = 1;
        result.descriptorType  = _bindings[i].descriptorType;
        result.dstBinding      = binding;
        result.dstSet          = set;

        _writes[writeIndex][i] = result;
        return i;
      }
    }

    RX_FATAL( "Failed to write binding to set. Binding could not be found." );
    return 0;
  }

  void Bindings::write( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, vk::WriteDescriptorSetAccelerationStructureKHR* pWriteDescriptorSetAccelerationStructureKHR )
  {
    for ( size_t i = 0; i < sets.size( ); ++i )
    {
      size_t j            = write( sets[i], i, binding );
      _writes[i][j].pNext = pWriteDescriptorSetAccelerationStructureKHR;
    }
  }

  void Bindings::write( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, vk::DescriptorImageInfo* pImageInfo )
  {
    for ( size_t i = 0; i < sets.size( ); ++i )
    {
      size_t j                 = write( sets[i], i, binding );
      _writes[i][j].pImageInfo = pImageInfo;
    }
  }

  void Bindings::write( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, vk::DescriptorBufferInfo* pBufferInfo )
  {
    for ( size_t i = 0; i < sets.size( ); ++i )
    {
      size_t j                  = write( sets[i], i, binding );
      _writes[i][j].pBufferInfo = pBufferInfo;
    }
  }

  void Bindings::write( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, const std::vector<vk::DescriptorBufferInfo>& uniformBufferInfos )
  {
    RX_ASSERT( uniformBufferInfos.size( ) == sets.size( ), "Failed to write descriptor for uniform buffer because there are not as many descriptor sets as uniform buffers." );

    for ( size_t i = 0; i < sets.size( ); ++i )
    {
      size_t j                  = write( sets[i], i, binding );
      _writes[i][j].pBufferInfo = &uniformBufferInfos[i];
    }
  }

  auto Bindings::writeArray( vk::DescriptorSet set, size_t writeIndex, uint32_t binding ) -> size_t
  {
    vk::WriteDescriptorSet result;

    for ( size_t i = 0; i < _bindings.size( ); ++i )
    {
      if ( _bindings[i].binding == binding )
      {
        result.descriptorCount = _bindings[i].descriptorCount;
        result.descriptorType  = _bindings[i].descriptorType;
        result.dstBinding      = binding;
        result.dstSet          = set;
        result.dstArrayElement = 0;

        _writes[writeIndex][i] = result;
        return i;
      }
    }

    RX_FATAL( "Failed to write binding to set. Binding could not be found." );
    return 0;
  }

  void Bindings::writeArray( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, const vk::DescriptorBufferInfo* pBufferInfo )
  {
    for ( size_t i = 0; i < sets.size( ); ++i )
    {
      size_t j                  = writeArray( sets[i], i, binding );
      _writes[i][j].pBufferInfo = pBufferInfo;
    }
  }

  void Bindings::writeArray( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, const vk::DescriptorImageInfo* pImageInfo )
  {
    for ( size_t i = 0; i < sets.size( ); ++i )
    {
      size_t j                 = writeArray( sets[i], i, binding );
      _writes[i][j].pImageInfo = pImageInfo;
    }
  }

  void Bindings::setPoolSizes( const std::vector<vk::DescriptorPoolSize>& poolSizes )
  {
    _poolSizes = poolSizes;
  }

  void Bindings::reset( )
  {
    _bindings.clear( );
    _flags.clear( );
    _flags.clear( );
    _poolSizes.reset( );
    _writes.clear( );
  };
} // namespace RAYEX_NAMESPACE
