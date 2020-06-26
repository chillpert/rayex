#ifndef SAMPLER_HPP
#define SAMPLER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct SamplerInfo
  {
    vk::Device device;
  };

  class Sampler
  {
  public:
    Sampler() = default;
    Sampler(SamplerInfo& info);
    Sampler(SamplerInfo&& info);
    RX_API ~Sampler();

    inline vk::Sampler get() const { return m_sampler; }

    void init(SamplerInfo& info);
    void init(SamplerInfo&& info);
    void destroy();

  private:
    vk::Sampler m_sampler;
    
    SamplerInfo m_info;
  };  
}

#endif // SAMPLER_HPP