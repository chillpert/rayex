#include "ImageViews.hpp"

namespace RX
{ 
  ImageViews::ImageViews() :
    BaseComponent("ImageViews") { }

  ImageViews::~ImageViews()
  {
    destroy();
  }

  void ImageViews::initialize(VkDevice device, VkFormat surfaceFormat, Images& images)
  {
    m_device = device;
    m_imageViews.resize(images.get().size());

    uint32_t temp = static_cast<uint32_t>(images.get().size());
    for (uint32_t i = 0; i < temp; ++i)
    {
      VkImageViewCreateInfo createInfo{ };
      createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      createInfo.image = images.get()[i];
      createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      createInfo.format = surfaceFormat;
      createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      createInfo.subresourceRange.baseMipLevel = 0;
      createInfo.subresourceRange.levelCount = 1;
      createInfo.subresourceRange.baseArrayLayer = 0;
      createInfo.subresourceRange.layerCount = 1;

      VK_ASSERT(vkCreateImageView(device, &createInfo, nullptr, &m_imageViews[i]), "Failed to create image view");
    }

    initializationCallback();
  }

  void ImageViews::destroy()
  {
    assertDestruction();

    for (auto imageView : m_imageViews)
      vkDestroyImageView(m_device, imageView, nullptr);
  }
}
