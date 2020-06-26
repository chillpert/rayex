#ifndef DESCRIPTOR_SET_HPP
#define DESCRIPTOR_SET_HPP

#include "DescriptorSetLayout.hpp"
#include "Buffer.hpp"
#include "UniformBuffer.hpp"
#include "Texture.hpp"

namespace RX
{
  struct DescriptorSetInfo
  {
    vk::Device device;
    vk::DescriptorPool pool;
    uint32_t setCount;
    std::vector<vk::DescriptorSetLayout> layouts;
  };

  struct SwapchainUpdateDescriptorSetInfo
  {
    vk::DescriptorPool descriptorPool;
    std::vector<vk::Buffer> uniformBuffers;
    vk::ImageView textureImageView = nullptr;
    vk::Sampler textureSampler = nullptr;
    vk::DescriptorSetLayout descriptorSetLayout;
  };

  struct UpdateRaytracingDescriptorSetInfo
  {
    vk::AccelerationStructureKHR tlas;
    vk::ImageView storageImageView;
  };

  class DescriptorSet
  {
  public:
    RX_API ~DescriptorSet();

    inline std::vector<vk::DescriptorSet>& get() { return m_sets; }
    inline vk::DescriptorSet& get(size_t index) { return m_sets[index]; }

    void init(DescriptorSetInfo& info);
    void update(SwapchainUpdateDescriptorSetInfo& info);
    void update(UpdateRaytracingDescriptorSetInfo& info);
    void destroy();

  private:
    std::vector<vk::DescriptorSet> m_sets;
    DescriptorSetInfo m_info;

    bool m_allocated = false;
  };
}

#endif // DESCRIPTOR_SET_HPP