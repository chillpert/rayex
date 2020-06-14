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
    std::vector<vk::DescriptorSetLayout> layouts;
  };

  struct SwapchainUpdateDescriptorSetInfo
  {
    std::vector<vk::Buffer> uniformBuffers;
    vk::ImageView textureImageView;
    vk::Sampler textureSampler;
    vk::DescriptorPool descriptorPool;
    vk::DescriptorSetLayout descriptorSetLayout;
  };

  class DescriptorSet
  {
  public:
    ~DescriptorSet();

    inline std::vector<vk::DescriptorSet> get() { return m_sets; }
    inline DescriptorSetInfo& getInfo() { return m_info; }

    void initialize(DescriptorSetInfo& info);
    void update(SwapchainUpdateDescriptorSetInfo& info);
    void destroy();

  private:
    std::vector<vk::DescriptorSet> m_sets;
    DescriptorSetInfo m_info;

    bool m_allocated = false;
  };
}

#endif // DESCRIPTOR_SET_HPP