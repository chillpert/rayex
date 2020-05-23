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

    VkDescriptorSetLayoutBinding layoutBinding{ };
    layoutBinding.binding = 0;
    layoutBinding.descriptorCount = 1;
    layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layoutBinding.pImmutableSamplers = nullptr;
    layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    createInfo.bindingCount = 1;
    createInfo.pBindings = &layoutBinding;

    VK_ASSERT(vkCreateDescriptorSetLayout(device, &createInfo, nullptr, &m_layout), "Failed to create descriptor set layout");

    RX_INITIALIZATION_CALLBACK;
  }

  void DescriptorSetLayout::destroy()
  {
    RX_ASSERT_DESTRUCTION;
    vkDestroyDescriptorSetLayout(m_device, m_layout, nullptr);
  }
}