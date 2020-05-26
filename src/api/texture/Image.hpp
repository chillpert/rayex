#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "api/BaseComponent.hpp"

namespace RX
{
  class Image : public BaseComponent
  {
  public:
    RX_API Image();
    RX_API virtual ~Image();

    virtual inline VkImage get() const { return m_image; }
    inline VkFormat getFormat() const { return m_format; }
    inline VkDevice getDevice() const { return m_device; }
    inline VkQueue getQueue() const { return m_queue; }
    inline VkCommandPool getCommandPool() const { return m_commandPool; }
    inline uint32_t getWidth() const { return m_width; }
    inline uint32_t getHeight() const { return m_height; }
    inline VkExtent2D getExtent() const { return { m_width, m_height }; }

    virtual void initialize(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool commandPool, VkImageCreateInfo& createInfo);
    virtual void transitionToLayout(VkImageLayout layout);
    
    static VkFormat findSupportedFormat(VkPhysicalDevice physicalDevice, const std::vector<VkFormat>& formatsToTest, VkFormatFeatureFlags features, VkImageTiling tiling);
    void destroy();
  
  protected:
    VkImage m_image;
    VkDeviceMemory m_memory;
    VkImageLayout m_layout;
    VkFormat m_format;

    uint32_t m_width;
    uint32_t m_height;

    VkPhysicalDevice m_physicalDevice;
    VkDevice m_device;
    VkQueue m_queue;
    VkCommandPool m_commandPool;
  };
}

#endif // IMAGE_HPP