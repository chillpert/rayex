#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Image.hpp"

namespace RX
{
  class Texture
  {
  public:
    void initialize(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool commandPool, const std::string& path);
  
  private:
    Image m_image;

  };
}

#endif // TEXTURE_HPP