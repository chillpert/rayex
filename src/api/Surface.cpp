#include "Surface.hpp"

namespace RX
{
  Surface::~Surface()
  {
    if (m_surface)
      destroy();
  }

  void Surface::initialize(SurfaceInfo& info)
  {
    m_info = info;

    m_surface = m_info.window->createSurface(m_info.instance);
    if (!m_surface)
      RX_ERROR("Failed to create surface.");
  }

  void Surface::destroy()
  {
    m_info.instance.destroySurfaceKHR(m_surface);
  }

  void Surface::checkSettingSupport(vk::PhysicalDevice physicalDevice)
  {
    // Get all surface capabilities.
    m_capabilities = physicalDevice.getSurfaceCapabilitiesKHR(m_surface);

    // Check a present mode.
    std::vector<vk::PresentModeKHR> presentModes = physicalDevice.getSurfacePresentModesKHR(m_surface);

    for (const auto& mode : presentModes)
    {
      if (mode == vk::PresentModeKHR::eMailbox)
      {
        m_info.presentMode = vk::PresentModeKHR::eMailbox;
        return;
      }
    }

    // Fall back, as FIFO is always supported on every device.
    m_info.presentMode = vk::PresentModeKHR::eFifo;

    // Check format and color space.
    auto formatProperties = physicalDevice.getFormatProperties(m_info.format); // TODO: not used.
    auto surfaceFormats = physicalDevice.getSurfaceFormatsKHR(m_surface);

    for (const auto& iter : surfaceFormats)
    {
      if (iter.format == m_info.format && iter.colorSpace == m_info.colorSpace)
        return;
    }

    // If the prefered format and color space are not available, fall back.
    m_info.format = surfaceFormats[0].format;
    m_info.colorSpace = surfaceFormats[0].colorSpace;
  }
}