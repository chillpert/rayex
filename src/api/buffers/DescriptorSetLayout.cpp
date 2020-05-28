#include "DescriptorSets.hpp"

namespace RX
{
  DescriptorSetLayout::DescriptorSetLayout() :
    BaseComponent("DescriptorSetLayout") { }

  DescriptorSetLayout::~DescriptorSetLayout()
  {
    destroy();
  }

  void DescriptorSetLayout::initialize(VkDevice device)
  {
    m_device = device;

    VkDescriptorSetLayoutBinding uboLayoutBinding{ };
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{ };
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
  
    VkDescriptorSetLayoutCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    createInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    createInfo.pBindings = bindings.data();

    VK_ASSERT(vkCreateDescriptorSetLayout(device, &createInfo, nullptr, &m_layout), "Failed to create descriptor set layout");

    RX_INITIALIZATION_CALLBACK;
  }

  void DescriptorSetLayout::destroy()
  {
    RX_DESTROY(vkDestroyDescriptorSetLayout(m_device, m_layout, nullptr), "layout");
  }
}