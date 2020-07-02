#include "Surface.hpp"
#include "Components.hpp"

namespace rx
{
  Surface::Surface() :
    m_format(vk::Format::eB8G8R8A8Unorm),
    m_colorSpace(vk::ColorSpaceKHR::eSrgbNonlinear),
    m_presentMode(vk::PresentModeKHR::eMailbox),
    m_capabilities(0) { }

  Surface::Surface(vk::Format format, vk::ColorSpaceKHR colorSpace, vk::PresentModeKHR presentMode, bool initialize) :
    m_format(vk::Format::eB8G8R8A8Unorm),
    m_colorSpace(vk::ColorSpaceKHR::eSrgbNonlinear),
    m_presentMode(vk::PresentModeKHR::eMailbox),
    m_capabilities(0)
  {
    if (initialize)
      init();
  }

  Surface::~Surface()
  {
    destroy();
  }

  void Surface::init()
  {
    m_surface = g_window->createSurface(g_instance);
    g_surface = m_surface;

    if (!m_surface)
      RX_ERROR("Failed to create surface.");
  }

  void Surface::checkSettingSupport()
  {
    // Get all surface capabilities.
    m_capabilities = g_physicalDevice.getSurfaceCapabilitiesKHR(m_surface);

    // Check a present mode.
    std::vector<vk::PresentModeKHR> presentModes = g_physicalDevice.getSurfacePresentModesKHR(m_surface);

    for (const auto& mode : presentModes)
    {
      if (mode == vk::PresentModeKHR::eMailbox)
      {
        m_presentMode = vk::PresentModeKHR::eMailbox;
        return;
      }
    }

    // Fall back, as FIFO is always supported on every device.
    m_presentMode = vk::PresentModeKHR::eFifo;

    // Check format and color space.
    auto formatProperties = g_physicalDevice.getFormatProperties(m_format); // TODO: not used.
    auto surfaceFormats = g_physicalDevice.getSurfaceFormatsKHR(m_surface);

    for (const auto& iter : surfaceFormats)
    {
      if (iter.format == m_format && iter.colorSpace == m_colorSpace)
        return;
    }

    // If the prefered format and color space are not available, fall back.
    m_format = surfaceFormats[0].format;
    m_colorSpace = surfaceFormats[0].colorSpace;
  }

  void Surface::destroy()
  {
    if (m_surface)
    {
      g_instance.destroySurfaceKHR(m_surface);
      m_surface = nullptr;
    }
  }
}