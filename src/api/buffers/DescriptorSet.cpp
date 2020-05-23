#include "DescriptorSet.hpp"

namespace RX
{
  DescriptorSet::DescriptorSet() :
    BaseComponent("DescriptorSet") { }

  DescriptorSet::~DescriptorSet()
  {
    destroy();
  }

  void DescriptorSet::initialize(VkDevice device)
  {
    m_device = device;

    VkDescriptorSetLayoutBinding layoutBinding{ };
    layoutBinding.binding = 0;
    layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layoutBinding.descriptorCount = 1;
    layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo layoutCreateInfo{ };
    layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutCreateInfo.bindingCount = 1;
    layoutCreateInfo.pBindings = &layoutBinding;

    VK_ASSERT(vkCreateDescriptorSetLayout(device, &layoutCreateInfo, nullptr, &m_descriptorSetLayout), "Failed to create descriptor set layout");
  }

  void DescriptorSet::destroy()
  {
    vkDestroyDescriptorSetLayout(m_device, m_descriptorSetLayout, nullptr);
  }
}