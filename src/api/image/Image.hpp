#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct ImageInfo
  {
    vk::PhysicalDevice physicalDevice;
    vk::Device device;
    vk::Queue queue; // Define in case you want to transition the image to another layout.
    vk::CommandPool commandPool; // Define in case you want to transition the image to another layout.
    vk::Extent3D extent;

    vk::ImageUsageFlags usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
    vk::Format format = vk::Format::eR8G8B8A8Srgb;
    vk::ImageTiling tiling = vk::ImageTiling::eOptimal;
    vk::ImageType imageType = vk::ImageType::e2D;
    vk::ImageLayout layout = vk::ImageLayout::eUndefined; // The initial image layout.
    uint32_t mipLevels = 1;
    uint32_t arrayLayers = 1;
    vk::SampleCountFlagBits samples = vk::SampleCountFlagBits::e1;
    vk::SharingMode sharingMode = vk::SharingMode::eExclusive;
  };

  class Image
  {
  public:
    Image() = default;
    Image(ImageInfo& info);
    ~Image();

    inline vk::Image get() { return m_image; }
    inline vk::Extent3D getExtent() const { return m_info.extent; }
    inline vk::DeviceMemory& getMemory() { return m_memory; }
    inline vk::Format getFormat() const { return m_info.format; }

    void initialize(ImageInfo& info);
    void destroy();

    void transitionToLayout(vk::ImageLayout layout);
    
    static vk::Format findSupportedFormat(vk::PhysicalDevice physicalDevice, const std::vector<vk::Format>& formatsToTest, vk::FormatFeatureFlagBits features, vk::ImageTiling tiling);
  
  protected:
    vk::Image m_image;
    vk::DeviceMemory m_memory;
    ImageInfo m_info;
  };
}

#endif // IMAGE_HPP