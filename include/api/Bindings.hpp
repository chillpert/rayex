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

    void update( )
    {
      for ( const auto& write : this->writes )
      {
        g_device.updateDescriptorSets( static_cast<uint32_t>( write.size( ) ), write.data( ), 0, nullptr );
      }
    }

    size_t writeToSet( vk::DescriptorSet set, size_t writesIndex, uint32_t binding, uint32_t arrayElement = 0 )
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
          
          this->writes[writesIndex][i] = result;
          return i;
        }
      }

      RX_ASSERT( false, "Failed to write binding to set. Binding could not be found." );
      return 0;
    }

    void writeToSet( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, const vk::WriteDescriptorSetAccelerationStructureKHR* pWriteDescriptorSetAccelerationStructureKHR )
    {
      for ( size_t i = 0; i < sets.size( ); ++i )
      {
        size_t j = writeToSet( sets[i], i, binding );
        this->writes[i][j].pNext = &pWriteDescriptorSetAccelerationStructureKHR;
      }
    }

    void writeToSet( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, const vk::DescriptorImageInfo* pImageInfo )
    {
      for ( size_t i = 0; i < sets.size( ); ++i )
      {
        size_t j = writeToSet( sets[i], i, binding );
        this->writes[i][j].pImageInfo = pImageInfo;
      }
    }

    void writeToSet( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, const vk::DescriptorBufferInfo* pBufferInfo )
    {
      for ( size_t i = 0; i < sets.size( ); ++i )
      {
        size_t j = writeToSet( sets[i], i, binding );
        this->writes[i][j].pBufferInfo = pBufferInfo;
      }
    }

    void setPoolSizes( const std::vector<vk::DescriptorPoolSize>& poolSizes )
    {
      this->poolSizes = poolSizes;
    }

  private:
    std::vector<vk::DescriptorSetLayoutBinding> bindings;
    std::vector<vk::DescriptorBindingFlags> flags;
    std::optional<std::vector<vk::DescriptorPoolSize>> poolSizes;

    std::vector<std::vector<vk::WriteDescriptorSet>> writes;
  };
}

#endif // BINDING_HPP