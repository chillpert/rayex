#ifndef SAMPLER_HPP
#define SAMPLER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class Sampler
  {
  public:
    inline vk::Sampler get() const { return m_sampler.get(); }
    void init();

  private:
    vk::UniqueSampler m_sampler;
  };  
}

#endif // SAMPLER_HPP