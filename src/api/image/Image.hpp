#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct ImageInfo
  {
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkQueue queue; // Define in case you want to transition the image to another layout.
    VkCommandPool commandPool; // Define in case you want to transition the image to another layout.
    VkExtent3D extent;

    VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;
    VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
    VkImageType imageType = VK_IMAGE_TYPE_2D;
    VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED; // The initial image layout.
    uint32_t mipLevels = 1;
    uint32_t arrayLayers = 1;
    VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
    VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  };

  class Image
  {
  public:
    RX_API ~Image();

    inline VkImage get() { return m_image; }
    inline VkDeviceMemory getMemory() { return m_memory; }
    inline ImageInfo& getInfo() { return m_info; }

    void initialize(ImageInfo& info);
    void transitionToLayout(VkImageLayout layout);
    
    static VkFormat findSupportedFormat(VkPhysicalDevice physicalDevice, const std::vector<VkFormat>& formatsToTest, VkFormatFeatureFlags features, VkImageTiling tiling);
    void destroy();
  
  protected:
    VkImage m_image;
    VkDeviceMemory m_memory;
    ImageInfo m_info;

    bool m_created = false;
  };
}

#endif // IMAGE_HPP