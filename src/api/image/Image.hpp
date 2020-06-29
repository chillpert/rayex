#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "Memory.hpp"

namespace RX
{
  class Image
  {
  public:
    Image() = default;
    Image(const vk::ImageCreateInfo& createInfo, bool initialize = true);

    inline const vk::Image get() const { return m_image.get(); }
    inline const vk::Extent3D getExtent() const { return m_extent; }
    inline const vk::Format getFormat() const { return m_format; }

    void init(const vk::ImageCreateInfo& createInfo);

    void transitionToLayout(const vk::ImageLayout& layout, const vk::CommandPool& cmdPool, const vk::Queue& queue);
    
    static vk::Format findSupportedFormat(vk::PhysicalDevice physicalDevice, const std::vector<vk::Format>& formatsToTest, vk::FormatFeatureFlagBits features, vk::ImageTiling tiling);
  
  protected:
    vk::UniqueImage m_image;
    Memory m_memory;

    vk::Extent3D m_extent;
    vk::Format m_format;
    vk::ImageLayout m_layout;
  };
}

#endif // IMAGE_HPP