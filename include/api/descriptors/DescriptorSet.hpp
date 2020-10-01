#ifndef DESCRIPTOR_SET_HPP
#define DESCRIPTOR_SET_HPP

#include "api/descriptors/DescriptorSetLayout.hpp"
#include "api/buffers/Buffer.hpp"
#include "api/buffers/UniformBuffer.hpp"
#include "api/image/Texture.hpp"

namespace RAYEXEC_NAMESPACE
{
  struct AccelerationStructureDescriptor
  { 
    vk::AccelerationStructureKHR accelerationStructure;
  };

  struct StorageImageDescriptor 
  { 
    vk::ImageView imageView;
  };

  struct UboDescriptor
  { 
    std::vector<vk::Buffer> uniformBuffers; 
    vk::DeviceSize size;
  };

  struct StorageBufferDescriptor
  {
    vk::Buffer storageBuffer;
    vk::DeviceSize size = VK_WHOLE_SIZE;
  };

  struct DynamicStorageBufferDescriptor
  {
    vk::Buffer storageBuffer;
    vk::DeviceSize size = VK_WHOLE_SIZE;
  };

  struct CombinedImageSamplerDescriptor
  {
    vk::ImageView imageView;
    vk::Sampler sampler;
  };

  struct StorageBufferArrayDescriptor
  {
    std::vector<vk::Buffer> storageBuffers;
    uint32_t descriptorCount;
    vk::DeviceSize size = VK_WHOLE_SIZE;
  };

  /// A wrapper class for Vulkan descriptor sets.
  /// @todo Make the update process "smart" by using the existing information to automatically write to and update the descriptor sets.
  /// @ingroup API
  class DescriptorSet
  {
  public:
    DescriptorSet( ) = default;

    /// @param descriptorPool The Vulkan descriptor pool to allocate the descriptor sets from.
    /// @param count The amount of Vulkan descriptor sets to create.
    /// @param layouts A vector of Vulkan descriptor set layouts.
    /// @param If true, the descriptor sets will be initialized right away without an additional call to init().
    DescriptorSet( vk::DescriptorPool descriptorPool, uint32_t count, const std::vector<vk::DescriptorSetLayout>& layouts, bool initialize = true );

    /// @return Returns a vector containing the Vulkan descriptor sets.
    inline const std::vector<vk::DescriptorSet> get( ) const { return sets; }

    /// Returns the Vulkan descriptor set at a given index.
    /// @param index The index of the descriptor set.
    /// @return Returns the Vulkan descriptor set.
    inline const vk::DescriptorSet get( size_t index ) const { return sets[index]; }

    /// Creates the descriptor sets.
    /// @param descriptorPool The Vulkan descriptor pool to allocate the descriptor sets from.
    /// @param count The amount of Vulkan descriptor sets to create.
    /// @param layouts A vector of Vulkan descriptor set layouts.
    void init( vk::DescriptorPool descriptorPool, uint32_t count, const std::vector<vk::DescriptorSetLayout>& layouts );

    /// Used to update ray tracing related descriptors.
    void update( const std::vector<std::any>& data );

    void update( const std::vector<vk::WriteDescriptorSet> writes );

    /// Frees the descriptor sets.
    void free( );

  private:
    std::vector<vk::DescriptorSet> sets; ///< A vector of Vulkan descriptor sets.
    std::vector<vk::DescriptorSetLayout> layouts; ///< A vector of Vulkan descriptor set layouts.
    std::vector<vk::DescriptorSetLayoutBinding> bindings;

    vk::DescriptorPool descriptorPool; ///< A Vulkan descriptor pool.
  };
}

#endif // DESCRIPTOR_SET_HPP