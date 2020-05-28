#ifndef SHADER_HPP
#define SHADER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct ShaderInfo
  {
    std::string fullPath;
    VkDevice device;
    uint32_t binding;
    uint32_t descriptorCount;
    VkDescriptorType descriptorType;
    VkShaderStageFlags stageFlags;
    VkSampler* pImmutableSamplers = nullptr;

    std::string pathToFile; // Ignore, will be initialized automatically.
    std::string fileName; // Ignore, will be initialized automatically.
    std::string fileNameOut; // Ignore, will be initialized automatically.
    std::vector<char> source; // Ignore, will be initialized automatically.
  };

  class Shader
  {
  public:
    ~Shader();

    inline VkShaderModule get() { return m_shaderModule; }
    inline ShaderInfo getInfo() { return m_info; }
    inline VkDescriptorSetLayoutBinding getDescriptorSetLayoutBinding() const { return { m_info.binding, m_info.descriptorType, m_info.descriptorCount, m_info.stageFlags, m_info.pImmutableSamplers }; }

    void initialize(ShaderInfo& info);
    void destroy();

  private:
    VkShaderModule m_shaderModule;
    ShaderInfo m_info;

    bool m_created = false;
  };
}

#endif // SHADER_HPP