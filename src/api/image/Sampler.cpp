#include "Sampler.hpp"
#include "Components.hpp"

namespace RX
{
  void Sampler::init()
  {
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

    m_sampler = g_device.createSamplerUnique(createInfo);
    if (!m_sampler)
      RX_ERROR("Failed to create sampler.");
  }
}