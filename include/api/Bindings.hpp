#ifndef BINDING_HPP
#define BINDING_HPP

#include "api/utility/Initializers.hpp"
#include "api/utility/Helpers.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// A utility class for managing descriptor related resources.
  /// @ingroup API
  /// @todo Documentation
  class Bindings
  {
  public:
    void add( uint32_t binding, vk::DescriptorType type, vk::ShaderStageFlags stage, uint32_t count = 1, vk::DescriptorBindingFlags flags = { } );

    vk::UniqueDescriptorSetLayout initLayoutUnique( );

    vk::UniqueDescriptorPool initPoolUnique( uint32_t maxSets, vk::DescriptorPoolCreateFlags flags = { } );

    std::vector<vk::DescriptorSet> initSets( const vk::UniqueDescriptorPool& pool, const vk::UniqueDescriptorSetLayout& layout );

    void update( );

    size_t write( vk::DescriptorSet set, size_t writesIndex, uint32_t binding, uint32_t arrayElement = 0 );

    void write( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, vk::WriteDescriptorSetAccelerationStructureKHR* pWriteDescriptorSetAccelerationStructureKHR );

    void write( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, vk::DescriptorImageInfo* pImageInfo );
    
    void write( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, vk::DescriptorBufferInfo* pBufferInfo );

    void write( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, const std::vector<vk::DescriptorBufferInfo>& uniformBufferInfos );

    void setPoolSizes( const std::vector<vk::DescriptorPoolSize>& poolSizes );

  private:
    std::vector<vk::DescriptorSetLayoutBinding> bindings;
    std::vector<vk::DescriptorBindingFlags> flags;
    std::optional<std::vector<vk::DescriptorPoolSize>> poolSizes;

    std::vector<std::vector<vk::WriteDescriptorSet>> writes;
  };
}

#endif // BINDING_HPP