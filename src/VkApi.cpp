#include "VkApi.hpp"
#include "utils/VkUtils.hpp"

namespace RX
{
  VkApi::VkApi(std::shared_ptr<Window> window) :
    m_window(window),
    m_instance(VK_NULL_HANDLE),
    m_physicalDevice(VK_NULL_HANDLE),
    m_device(VK_NULL_HANDLE),
    m_surface(VK_NULL_HANDLE),
    m_swapChain(VK_NULL_HANDLE),
    m_semaphore(VK_NULL_HANDLE),
    m_queue(VK_NULL_HANDLE) { }

  void VkApi::initialize()
  {
    uint32_t familyIndex = 0;

    m_instance = createInstance(m_window);
    m_physicalDevice = pickPhysicalDevice(m_instance);
    m_device = createDevice(m_instance, m_physicalDevice, &familyIndex);
    m_surface = m_window->createSurface(m_instance);
    m_swapChain = createSwapChain(m_physicalDevice, m_device, m_surface, m_window, &familyIndex);

    vkGetDeviceQueue(m_device, familyIndex, 0, &m_queue);

    uint32_t swapChainImageCount;
    vkGetSwapchainImagesKHR(m_device, m_swapChain, &swapChainImageCount, nullptr);

    std::vector<VkImage> swapChainImages(swapChainImageCount);
    vkGetSwapchainImagesKHR(m_device, m_swapChain, &swapChainImageCount, swapChainImages.data());

    /*
    Here is an image index that you can use in the future.
    But it's used conditional on the fact, that the GPU stops using this image before
    you start using it again.
    And the way how you know when it stopped is using the semaphore.
    */
  }

  bool VkApi::update()
  {
    return true;
  }

  bool VkApi::render()
  {
    uint32_t imageIndex = 0;
    vkAcquireNextImageKHR(m_device, m_swapChain, VK_TIMEOUT, m_semaphore, VK_NULL_HANDLE, &imageIndex);

    VkPresentInfoKHR presentInfo = { };
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_swapChain;
    presentInfo.pImageIndices = &imageIndex;

    

    Assert::vulkan(
      vkQueuePresentKHR(m_queue, &presentInfo),
      "Failed to present"
    );

    Assert::vulkan(
      vkDeviceWaitIdle(m_device),
      "Device failed to wait idle"
    );

    return true;
  }

  void VkApi::clean()
  {

  }
}