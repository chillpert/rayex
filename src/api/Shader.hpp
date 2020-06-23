#ifndef SHADER_HPP
#define SHADER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct ShaderInfo
  {
    std::string fullPath;
    vk::Device device;
    uint32_t binding;
    uint32_t descriptorCount;
    vk::DescriptorType descriptorType;
    vk::ShaderStageFlags stageFlags;
    vk::Sampler* pImmutableSamplers = nullptr;

    std::string pathToFile; // Ignore, will be initialized automatically.
    std::string fileName; // Ignore, will be initialized automatically.
    std::string fileNameOut; // Ignore, will be initialized automatically.
    std::vector<char> source; // Ignore, will be initialized automatically.
  };

  class Shader
  {
  public:
    ~Shader();

    inline vk::ShaderModule get() { return m_shaderModule; }
    inline vk::DescriptorSetLayoutBinding getDescriptorSetLayoutBinding() const { return { m_info.binding, m_info.descriptorType, m_info.descriptorCount, m_info.stageFlags, m_info.pImmutableSamplers }; }

    void initialize(ShaderInfo& info);
    void destroy();

  private:
    vk::ShaderModule m_shaderModule;
    ShaderInfo m_info;
  };
}

#endif // SHADER_HPP