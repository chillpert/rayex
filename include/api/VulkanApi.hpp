#ifndef VULKAN_API_HPP
#define VULKAN_API_HPP

#include "api/Api.hpp"

namespace RX
{
  class VulkanApi : public Api
  {
  private:
    bool initialize() override;
    void update() override;
    void render() override;
    void clean() override;
  };
}

#endif // VULKAN_API_HPP