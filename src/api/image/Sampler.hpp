#ifndef SAMPLER_HPP
#define SAMPLER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class Sampler
  {
  public:
    RX_API ~Sampler();

    inline vk::Sampler get() const { return m_sampler; }

    void initialize(vk::Device device);
    void destroy();

  private:
    vk::Sampler m_sampler;
    vk::Device m_device;

    bool m_created = false;
  };  
}

#endif // SAMPLER_HPP