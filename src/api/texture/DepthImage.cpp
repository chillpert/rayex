#include "DepthImage.hpp"

namespace RX
{
  void DepthImage::initialize(VkPhysicalDevice physicalDevice, VkDevice device, VkExtent2D swapchainExtent)
  {
    m_format = getSupportedFormat(physicalDevice);

    VkImageCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    createInfo.imageType = VK_IMAGE_TYPE_2D;
    createInfo.extent.width = swapchainExtent.width;
    createInfo.extent.height = swapchainExtent.height;
    createInfo.extent.depth = 1;
    createInfo.mipLevels = 1;
    createInfo.arrayLayers = 1;
    createInfo.format = m_format;
    createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    createInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    Image::initialize(physicalDevice, device, VK_NULL_HANDLE, VK_NULL_HANDLE, createInfo);

    m_imageView.initialize(device, get(), getFormat(), VK_IMAGE_ASPECT_DEPTH_BIT);
  }

  void DepthImage::transitionToLayout(VkImageLayout layout)
  {
    RX_ERROR("Transition to layout not implemented for a depth image.");
  }

  void DepthImage::destroy()
  {
    m_imageView.destroy();
    Image::destroy();
  }

  VkFormat DepthImage::getSupportedFormat(VkPhysicalDevice physicalDevice)
  {
    std::vector<VkFormat> candidates{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
    return findSupportedFormat(physicalDevice, candidates, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_TILING_OPTIMAL);
  }
}