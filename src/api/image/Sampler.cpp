#include "Sampler.hpp"

namespace RX
{
  Sampler::~Sampler()
  {
    if (m_sampler)
      destroy();
  }

  void Sampler::initialize(SamplerInfo& info)
  {
    m_info = info;

    vk::SamplerCreateInfo createInfo;
    createInfo.magFilter = vk::Filter::eLinear;
    createInfo.minFilter = vk::Filter::eLinear;
    createInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
    createInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
    createInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
    createInfo.anisotropyEnable = VK_TRUE;
    createInfo.maxAnisotropy = 16.0f;
    createInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
    createInfo.unnormalizedCoordinates = VK_FALSE;
    createInfo.compareEnable = VK_FALSE;
    createInfo.compareOp = vk::CompareOp::eAlways;
    createInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;

    m_sampler = m_info.device.createSampler(createInfo);
    if (!m_sampler)
      RX_ERROR("Failed to create sampler.");
  }

  void Sampler::destroy()
  {
    m_info.device.destroySampler(m_sampler);
    m_sampler = nullptr;
  }
}