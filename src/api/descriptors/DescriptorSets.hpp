#ifndef DESCRIPTOR_SETS_HPP
#define DESCRIPTOR_SETS_HPP

#include "DescriptorSetLayout.hpp"
#include "Buffer.hpp"
#include "UniformBuffer.hpp"
#include "Texture.hpp"

namespace RX
{
  class DescriptorSets
  {
  public:
    inline std::vector<VkDescriptorSet> get() { return m_sets; }

    void initialize(VkDevice device, size_t swapchainImagesCount, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, std::vector<Buffer>& uniformBuffers, Texture& texture);

  private:
    std::vector<VkDescriptorSet> m_sets;
  };
}

#endif // DESCRIPTOR_SETS_HPP