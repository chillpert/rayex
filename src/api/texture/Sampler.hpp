#ifndef SAMPLER_HPP
#define SAMPLER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class Sampler
  {
  public:
    RX_API ~Sampler();

    inline VkSampler get() const { return m_sampler; }

    void initialize(VkDevice device);
    void destroy();

  private:
    VkSampler m_sampler;
    VkDevice m_device;

    bool m_created = false;
  };  
}

#endif // SAMPLER_HPP