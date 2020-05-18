#include "Images.hpp"

namespace RX
{
  void Images::initialize(VkDevice device, VkSwapchainKHR swapchain)
  {
    uint32_t imageCount;
    VK_ASSERT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr), "Failed to get swap chain images");

    m_images.resize(imageCount);
    VK_ASSERT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, m_images.data()), "Failed to get swap chain images");
  }
}
