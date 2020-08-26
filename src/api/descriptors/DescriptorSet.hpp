#ifndef DESCRIPTOR_SET_HPP
#define DESCRIPTOR_SET_HPP

#include "DescriptorSetLayout.hpp"
#include "Buffer.hpp"
#include "UniformBuffer.hpp"
#include "Texture.hpp"

namespace rx
{
  class DescriptorSet
  {
  public:
    DescriptorSet( ) = default;
    DescriptorSet( vk::DescriptorPool descriptorPool, uint32_t count, const std::vector<vk::DescriptorSetLayout>& layouts, bool initialize = true );

    inline const std::vector<vk::DescriptorSet> get( ) const { return m_sets; }
    inline const vk::DescriptorSet get( size_t index ) const { return m_sets[index]; }

    void init( vk::DescriptorPool descriptorPool, uint32_t count, const std::vector<vk::DescriptorSetLayout>& layouts );

    void update( vk::AccelerationStructureKHR tlas, vk::ImageView storageImageView, const std::vector<vk::Buffer>& uniformBuffers );
    void update( vk::ImageView textureImageView, vk::Sampler textureSampler );
    void update( vk::Buffer vertexBuffer, vk::Buffer indexBuffer );

    void free( );

  private:
    vk::WriteDescriptorSet writeUniformBuffer( vk::DescriptorSet descriptorSet, uint32_t binding, const vk::DescriptorBufferInfo& bufferInfo );
    vk::WriteDescriptorSet writeStorageBuffer( vk::DescriptorSet descriptorSet, uint32_t binding, const vk::DescriptorBufferInfo& bufferInfo );
    vk::WriteDescriptorSet writeStorageImage( vk::DescriptorSet descriptorSet, uint32_t binding, const vk::DescriptorImageInfo& imageInfo );
    vk::WriteDescriptorSet writeCombinedImageSampler( vk::DescriptorSet descriptorSet, uint32_t binding, const vk::DescriptorImageInfo& imageInfo );
    vk::WriteDescriptorSet writeAccelerationStructure( vk::DescriptorSet descriptorSet, uint32_t binding, void* pNext );

    std::vector<vk::DescriptorSet> m_sets;
    std::vector<vk::DescriptorSetLayout> m_layouts;

    vk::DescriptorPool m_descriptorPool;
  };
}

#endif // DESCRIPTOR_SET_HPP