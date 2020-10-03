#ifndef BINDING_HPP
#define BINDING_HPP

#include "api/utility/Helpers.hpp"
#include "api/utility/Initializers.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// A utility class for managing descriptor related resources.
  /// @ingroup API
  /// @todo Documentation
  class Bindings
  {
  public:
    void add( uint32_t binding, vk::DescriptorType type, vk::ShaderStageFlags stage, uint32_t count = 1, vk::DescriptorBindingFlags flags = { } );

    auto initLayoutUnique( ) -> vk::UniqueDescriptorSetLayout;

    auto initPoolUnique( uint32_t maxSets, vk::DescriptorPoolCreateFlags flags = { } ) -> vk::UniqueDescriptorPool;

    void update( );

    auto write( vk::DescriptorSet set, size_t writesIndex, uint32_t binding, uint32_t arrayElement = 0 ) -> size_t;

    void write( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, vk::WriteDescriptorSetAccelerationStructureKHR* pWriteDescriptorSetAccelerationStructureKHR );

    void write( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, vk::DescriptorImageInfo* pImageInfo );

    void write( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, vk::DescriptorBufferInfo* pBufferInfo );

    void write( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, const std::vector<vk::DescriptorBufferInfo>& uniformBufferInfos );

    auto writeArray( vk::DescriptorSet set, size_t writesIndex, uint32_t binding ) -> size_t;

    void writeArray( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, vk::DescriptorBufferInfo* pBufferInfo );

    void setPoolSizes( const std::vector<vk::DescriptorPoolSize>& poolSizes );

  private:
    std::vector<vk::DescriptorSetLayoutBinding> bindings;
    std::vector<vk::DescriptorBindingFlags> flags;
    std::optional<std::vector<vk::DescriptorPoolSize>> poolSizes;

    std::vector<std::vector<vk::WriteDescriptorSet>> writes;
  };
} // namespace RAYEXEC_NAMESPACE

#endif // BINDING_HPP