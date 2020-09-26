#ifndef DESCRIPTOR_HPP
#define DESCRIPTOR_HPP

#include "pch/stdafx.hpp"

namespace RAYEXEC_NAMESPACE
{
  struct CombinedImageSampler
  {
    vk::ImageView imageView;
    vk::Sampler sampler;
  };

  struct Binding
  {
    uint32_t binding;
    vk::DescriptorType descriptorType;
    vk::ShaderStageFlags shaderStage;
    vk::DeviceSize size = 0;
  };

  const size_t maxDescriptorCount = 10;

  /// @todo Major flaw: Remove option to set binding. Binding is going to be incremented by one for each push call. And dont forget to reset the thing again in destroy.
  /// @todo Combine setBindings with init and remove init argument.
  class Descriptors
  {
  public:
    ~Descriptors( );

    const vk::DescriptorSetLayout getLayout( ) const { return this->layout; }
    const vk::DescriptorPool getPool( ) const { return this->pool; }
    const std::vector<vk::DescriptorSet>& getSets( ) const { return this->sets; }
    const vk::DescriptorSet getSet( size_t index ) const { return this->sets[index]; }

    void setBinding( const Binding& binding );

    void init( const std::vector<Binding>& descriptorBindings );
    void init( const std::vector<Binding>& descriptorBindings, std::vector<vk::DescriptorSet>& externalDescriptorSets );
    void update( const std::vector<std::any>& descriptors );

    void destroy( );

  private:
    vk::DescriptorSet determineSet( size_t setIndex, size_t bindingIndex );

    vk::WriteDescriptorSet writeBuffer( vk::DescriptorBufferInfo info, size_t setIndex, size_t bindingIndex );
    vk::WriteDescriptorSet writeImage( vk::DescriptorImageInfo info, size_t setIndex, size_t bindingIndex );
    vk::WriteDescriptorSet writeAccelerationStructure( vk::WriteDescriptorSetAccelerationStructureKHR info, size_t setIndex, size_t bindingIndex );

    vk::DescriptorSetLayout layout;
    std::vector<vk::DescriptorSetLayout> layouts;
    vk::DescriptorPool pool;
    std::vector<vk::DescriptorSet> sets;
    std::vector<vk::DescriptorSet>* externalSets = nullptr;

    std::vector<vk::DescriptorSetLayoutBinding> bindings;
    std::vector<vk::DeviceSize> sizes;

    bool initialized = false;
    size_t counter = 0;
  };
}

#endif // DESCRIPTOR_HPP