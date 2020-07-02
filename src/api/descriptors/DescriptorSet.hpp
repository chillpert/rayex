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

    void update( const std::vector<vk::Buffer>& uniformBuffers, vk::ImageView textureImageView, vk::Sampler textureSampler );
    void update( vk::AccelerationStructureKHR tlas, vk::ImageView storageImageView );

    void free( );

  private:
    std::vector<vk::DescriptorSet> m_sets;
    std::vector<vk::DescriptorSetLayout> m_layouts;

    vk::DescriptorPool m_descriptorPool;
  };
}

#endif // DESCRIPTOR_SET_HPP