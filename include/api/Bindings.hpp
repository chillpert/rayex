#ifndef BINDING_HPP
#define BINDING_HPP

#include "api/utility/Initializers.hpp"
#include "api/utility/Helpers.hpp"

namespace RAYEXEC_NAMESPACE
{
  class Bindings
  {
  public:
    void add( uint32_t binding, vk::DescriptorType type, vk::ShaderStageFlags stage, uint32_t count = 1, vk::DescriptorBindingFlags flags = { } )
    {
      vk::DescriptorSetLayoutBinding temp( binding,   // binding
                                           type,      // descriptorType
                                           count,     // descriptorCount
                                           stage,     // stageFlags
                                           nullptr ); // pImmutableSamplers

      this->bindings.push_back( temp );
      this->flags.push_back( flags );
    }

    bool initLayoutUnique( vk::UniqueDescriptorSetLayout& layout )
    {
      uint32_t bindingCount = static_cast<uint32_t>( this->bindings.size( ) );

      vk::DescriptorSetLayoutCreateInfo createInfo( { },                      // flags
                                                    bindingCount,             // bindingCount
                                                    this->bindings.data( ) ); // pBindings 

      vk::DescriptorSetLayoutBindingFlagsCreateInfoEXT layoutFlags( bindingCount,          // bindingCount 
                                                                    this->flags.data( ) ); // pBindingFlags
      createInfo.pNext = &layoutFlags;

      layout = g_device.createDescriptorSetLayoutUnique( createInfo );
      if ( !layout )
      {
        RX_ERROR( "Failed to create descriptor set layout." );
        return false;
      }

      return true;
    }

    bool initPoolUnique( vk::UniqueDescriptorPool& pool, uint32_t maxSets, vk::DescriptorPoolCreateFlags flags = { } )
    {
      std::vector<vk::DescriptorPoolSize> tPoolSizes;

      if ( this->poolSizes.has_value( ) )
        tPoolSizes = this->poolSizes.value( );
      else
        tPoolSizes = vk::Helper::getPoolSizes( this->bindings, maxSets );
       
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

      pool = g_device.createDescriptorPoolUnique( createInfo );
      if ( !pool )
      {
        RX_ERROR( "Failed to create descriptor pool." );
        return false;
      }

      return true;
    }

    vk::WriteDescriptorSet writeToSet( vk::DescriptorSet set, uint32_t binding, uint32_t arrayElement = 0 )
    {
      vk::WriteDescriptorSet result;

      for ( size_t i = 0; i < this->bindings.size( ); ++i )
      {
        if ( this->bindings[i].binding == binding )
        {
          result.descriptorCount = 1;
          result.descriptorType = this->bindings[i].descriptorType;
          result.dstBinding = binding;
          result.dstSet = set;
          result.dstArrayElement = arrayElement;
          return result;
        }
      }

      RX_ASSERT( false, "Failed to write binding to set. Binding could not be found." );
      return result;
    }

    vk::WriteDescriptorSet writeToSet( vk::DescriptorSet set, uint32_t binding, const vk::WriteDescriptorSetAccelerationStructureKHR* pWriteDescriptorSetAccelerationStructureKHR )
    {
      vk::WriteDescriptorSet result = writeToSet( set, binding );
      result.pNext = &pWriteDescriptorSetAccelerationStructureKHR;
      return result;
    }

    vk::WriteDescriptorSet writeToSet( vk::DescriptorSet set, uint32_t binding, const vk::DescriptorImageInfo* pImageInfo )
    {
      vk::WriteDescriptorSet result = writeToSet( set, binding );
      result.pImageInfo = pImageInfo;
      return result;
    }

    vk::WriteDescriptorSet writeToSet( vk::DescriptorSet set, uint32_t binding, const vk::DescriptorBufferInfo* pBufferInfo )
    {
      vk::WriteDescriptorSet result = writeToSet( set, binding );
      result.pBufferInfo = pBufferInfo;
      return result;
    }

    void setPoolSizes( const std::vector<vk::DescriptorPoolSize>& poolSizes )
    {
      this->poolSizes = poolSizes;
    }

  private:
    std::vector<vk::DescriptorSetLayoutBinding> bindings;
    std::vector<vk::DescriptorBindingFlags> flags;
    std::optional<std::vector<vk::DescriptorPoolSize>> poolSizes;
  };
}

#endif // BINDING_HPP