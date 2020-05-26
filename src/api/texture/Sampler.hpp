#ifndef SAMPLER_HPP
#define SAMPLER_HPP

#include "api/BaseComponent.hpp"

namespace RX
{
  class Sampler : public BaseComponent
  {
  public:
    RX_API Sampler();
    RX_API ~Sampler();

    inline VkSampler get() const { return m_sampler; }

    void initialize(VkDevice device);
    void destroy();

  private:
    VkSampler m_sampler;
    VkDevice m_device;
  };  
}

#endif // SAMPLER_HPP