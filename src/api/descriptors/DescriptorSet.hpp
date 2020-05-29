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
    std::vector<VkBuffer> uniformBuffers;
    VkImageView textureImageView;
    VkSampler textureSampler;
    VkDevice device; 
    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;
    size_t swapchainImagesCount; 
  };

  class DescriptorSet
  {
  public:
    ~DescriptorSet();

    inline std::vector<VkDescriptorSet> get() { return m_sets; }
    inline DescriptorSetInfo& getInfo() { return m_info; }

    void initialize(DescriptorSetInfo& info);
    void destroy();

  private:
    std::vector<VkDescriptorSet> m_sets;
    DescriptorSetInfo m_info;

    bool m_allocated = false;
  };
}

#endif // DESCRIPTOR_SET_HPP