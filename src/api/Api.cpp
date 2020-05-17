#include "api/Api.hpp"

namespace RX
{
  Api::Api(std::shared_ptr<Window> window) :
    m_window(window) { }

  void Api::initialize()
  {
    // Required for extending the physical m_device from m_device extensions.
    m_instance.pushExtension("VK_KHR_get_physical_device_properties2");
    m_instance.initialize(m_window);
    //m_instance.print();

    m_debugMessenger.initialize(m_instance.get());

    m_surface.initialize(m_instance.get(), m_window);

    m_physicalDevice.initialize(m_instance.get(), m_surface.get());

    // Extensions required for ray tracing.
    std::vector<const char*> requiredExtensions =
    {
      "VK_KHR_get_memory_requirements2",
      "VK_EXT_descriptor_indexing",
      "VK_KHR_buffer_device_address",
      "VK_KHR_deferred_host_operations",
      "VK_KHR_pipeline_library",
      "VK_KHR_ray_tracing"
    };

    m_physicalDevice.checkExtensionSupport(requiredExtensions);

    // Set up queues.
    m_queueManager.initialize(m_physicalDevice.get(), m_surface.get());

    // Add all of the device extensions from above.
    //for (const auto& extension : requiredExtensions)
    //  m_device.pushExtension(extension);

    m_device.initialize(m_physicalDevice.get(), m_queueManager);

    m_swapchain.initialize(m_physicalDevice.get(), m_device.get(), m_surface, m_window, m_queueManager);

    m_imageAvailableSemaphore.initialize(m_device.get());
    m_finishedRenderSemaphore.initialize(m_device.get());
    
    m_renderPass.initialize(m_device.get(), m_surface.getFormat().format);
    
    Shader vs, fs;
    vs.initialize(RX_SHADER_PATH "test.vert", m_device.get());
    fs.initialize(RX_SHADER_PATH "test.frag", m_device.get());
    m_pipeline.initialize(m_device.get(), m_renderPass.get(), m_swapchain.getExtent(), m_window, vs, fs);
    
    m_swapchain.initializeImages(m_device.get());
    m_swapchain.initializeImageViews(m_device.get(), m_surface);
    m_swapchain.initializeFramebuffers(m_device.get(), m_renderPass.get(), m_window);
    m_commandPool.initialize(m_device.get(), m_queueManager.getGraphicsIndex());
    m_commandBuffers.initialize(m_device.get(), m_commandPool.get(), m_swapchain.getFramebuffers().size());
  }

  bool Api::update()
  {
    return true;
  }

  bool Api::render()
  {
    

    return true;
  }

  void Api::clean()
  {
    m_debugMessenger.destroy(m_instance.get());
  }
}