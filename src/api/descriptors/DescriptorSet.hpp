#ifndef DESCRIPTOR_SET_HPP
#define DESCRIPTOR_SET_HPP

#include "DescriptorSetLayout.hpp"
#include "Buffer.hpp"
#include "UniformBuffer.hpp"
#include "Texture.hpp"

namespace rx
{
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
    inline const std::vector<vk::DescriptorSet> get( ) const { return m_sets; }

    /// Returns the Vulkan descriptor set at a given index.
    /// @param index The index of the descriptor set.
    /// @return Returns the Vulkan descriptor set.
    inline const vk::DescriptorSet get( size_t index ) const { return m_sets[index]; }

    /// Creates the descriptor sets.
    /// @param descriptorPool The Vulkan descriptor pool to allocate the descriptor sets from.
    /// @param count The amount of Vulkan descriptor sets to create.
    /// @param layouts A vector of Vulkan descriptor set layouts.
    void init( vk::DescriptorPool descriptorPool, uint32_t count, const std::vector<vk::DescriptorSetLayout>& layouts );

    /// Used to update ray tracing related descriptors.
    /// @param tlas The top level acceleration structure.
    /// @param storageImageView The storage image view the ray tracing shaders will write to.
    /// @param uniformBuffers A vector of uniform buffers used to upload camera data.
    void update( const vk::AccelerationStructureKHR& tlas, vk::ImageView storageImageView, const std::vector<vk::Buffer>& uniformBuffers );
    
    /// Used to update model related descriptors (with textures).
    /// @param textureImageView A texture's image view.
    /// @param textureSampler A texture's sampler.
    /// @param vertexBuffer A Vulkan buffer containing the vertex data of a model.
    /// @param indexBuffer A Vulkan buffer containing the index data of a model.
    void update( vk::ImageView textureImageView, vk::Sampler textureSampler, vk::Buffer vertexBuffer, vk::Buffer indexBuffer );

    /// Used to update model related descriptors (without textures).
    /// @param vertexBuffer A Vulkan buffer containing the vertex data of a model.
    /// @param indexBuffer A Vulkan buffer containing the index data of a model.
    /// @todo update documentation
    void update( vk::Buffer uniformBuffer1, vk::Buffer uniformBuffer2 );

    /// Frees the descriptor sets.
    void free( );

  private:
    /// Creates a Vulkan write descriptor set object for uniform buffers.
    /// @param descriptorSet The Vulkan descriptor set to create the write descriptor set object for.
    /// @param binding The binding in the shader.
    /// @param bufferInfo The Vulkan descriptor buffer info for the uniform buffer.
    vk::WriteDescriptorSet writeUniformBuffer( vk::DescriptorSet descriptorSet, uint32_t binding, const vk::DescriptorBufferInfo& bufferInfo );
    
    /// Creates a Vulkan write descriptor set object for storage buffers.
    /// @param descriptorSet The Vulkan descriptor set to create the write descriptor set object for.
    /// @param binding The binding in the shader.
    /// @param bufferInfo The Vulkan descriptor buffer info for the storage buffer.
    vk::WriteDescriptorSet writeStorageBuffer( vk::DescriptorSet descriptorSet, uint32_t binding, const vk::DescriptorBufferInfo& bufferInfo );

    /// Creates a Vulkan write descriptor set object for storage images.
    /// @param descriptorSet The Vulkan descriptor set to create the write descriptor set object for.
    /// @param binding The binding in the shader.
    /// @param imageInfo The Vulkan descriptor buffer info for the storage images.
    vk::WriteDescriptorSet writeStorageImage( vk::DescriptorSet descriptorSet, uint32_t binding, const vk::DescriptorImageInfo& imageInfo );

    /// Creates a Vulkan write descriptor set object for combined image samplers.
    /// @param descriptorSet The Vulkan descriptor set to create the write descriptor set object for.
    /// @param binding The binding in the shader.
    /// @param imageInfo The Vulkan descriptor buffer info for the combined image samplers.
    vk::WriteDescriptorSet writeCombinedImageSampler( vk::DescriptorSet descriptorSet, uint32_t binding, const vk::DescriptorImageInfo& imageInfo );

    /// Creates a Vulkan write descriptor set object for acceleration structures.
    /// @param descriptorSet The Vulkan descriptor set to create the write descriptor set object for.
    /// @param binding The binding in the shader.
    /// @param pNext A pointer to the pNext chain of the write descriptor set object.
    vk::WriteDescriptorSet writeAccelerationStructure( vk::DescriptorSet descriptorSet, uint32_t binding, void* pNext );

    std::vector<vk::DescriptorSet> m_sets; ///< A vector of Vulkan descriptor sets.
    std::vector<vk::DescriptorSetLayout> m_layouts; ///< A vector of Vulkan descriptor set layouts.

    vk::DescriptorPool m_descriptorPool; ///< A Vulkan descriptor pool.


    std::vector<const DescriptorSetLayout*> m_descriptorSetLayouts;
  };
}

#endif // DESCRIPTOR_SET_HPP