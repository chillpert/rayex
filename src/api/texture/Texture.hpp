#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "api/buffers/Buffer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

namespace RX
{
  class Texture
  {
  public:
    void initialize(VkPhysicalDevice physicalDevice, VkDevice device, const std::string& path);
  
  private:
    Buffer m_buffer;
  };
}

#endif // TEXTURE_HPP