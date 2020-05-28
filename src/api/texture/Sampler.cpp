#include "Sampler.hpp"

namespace RX
{
  Sampler::~Sampler()
  {
    destroy();
  }

  void Sampler::initialize(VkDevice device)
  {
    m_device = device;

    VkSamplerCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    createInfo.magFilter = VK_FILTER_LINEAR;
    createInfo.minFilter = VK_FILTER_LINEAR;
    createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    createInfo.anisotropyEnable = VK_TRUE;
    createInfo.maxAnisotropy = 16.0f;
    createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    createInfo.unnormalizedCoordinates = VK_FALSE;
    createInfo.compareEnable = VK_FALSE;
    createInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    VK_CREATE(vkCreateSampler(device, &createInfo, nullptr, &m_sampler), "sampler");
  }

  void Sampler::destroy()
  {
    VK_DESTROY(vkDestroySampler(m_device, m_sampler, nullptr), "sampler");
  }
}