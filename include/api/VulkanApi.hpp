#ifndef VULKAN_API_HPP
#define VULKAN_API_HPP

#include "api/Api.hpp"

namespace RX
{
  class VulkanApi : public Api
  {
  private:
    RX_API bool initialize(std::shared_ptr<Window> window) override;
    RX_API void update() override;
    RX_API void render() override;
    RX_API void clean() override;
  };
}

#endif // VULKAN_API_HPP