#ifndef DESCRIPTOR_HPP
#define DESCRIPTOR_HPP

#include "pch/stdafx.hpp"

namespace RAYEXEC_NAMESPACE
{
  class DescriptorManager
  {
  public:
    ~DescriptorManager( );

    const vk::DescriptorSetLayout getLayout( ) const { return this->layout; }
    const std::vector<vk::DescriptorSet>& getSets( ) const { return this->sets; }
    const vk::DescriptorSet getSet( size_t index ) const { return this->sets[index]; }

    void addDescriptor( const std::any& descriptor, uint32_t binding, vk::DescriptorType descriptorType, vk::ShaderStageFlags shaderStage, vk::DeviceSize size );

    void init( uint32_t maxSets );
    void update( );

    void destroy( );

  private:
    vk::DescriptorSetLayout layout;
    std::vector<vk::DescriptorSetLayout> layouts;
    vk::DescriptorPool pool;
    std::vector<vk::DescriptorSet> sets;

    std::vector<std::any> descriptors;
    std::vector<vk::DescriptorSetLayoutBinding> bindings;
    std::vector<vk::DeviceSize> sizes;

    bool initialized = false;
  };
}

#endif // DESCRIPTOR_HPP