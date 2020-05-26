#ifndef API_HPP
#define API_HPP

#include "Instance.hpp"
#include "DebugMessenger.hpp"
#include "Queues.hpp"
#include "Surface.hpp"
#include "Pipeline.hpp"
#include "RenderPass.hpp"
#include "Shader.hpp"
#include "Model.hpp"

#include "devices/PhysicalDevice.hpp"
#include "devices/Device.hpp"

#include "swapchain/Swapchain.hpp"
#include "swapchain/Images.hpp"
#include "swapchain/ImageViews.hpp"
#include "swapchain/Framebuffers.hpp"

#include "sync/Semaphore.hpp"
#include "sync/Fence.hpp"

#include "texture/Texture.hpp"
#include "texture/DepthImage.hpp"

#include "buffers/CommandPool.hpp"
#include "buffers/CommandBuffers.hpp"
#include "buffers/VertexBuffer.hpp"
#include "buffers/IndexBuffer.hpp"
#include "buffers/DescriptorSets.hpp"
#include "buffers/DescriptorPool.hpp"
#include "buffers/DescriptorSetLayout.hpp"
#include "buffers/UniformBuffers.hpp"

#include "gui/Gui.hpp"

namespace RX
{
  class Api
  {
  public:
    Api(std::shared_ptr<Window> window);
    RX_API ~Api();

    void initialize();
    bool update();
    bool render();
  
    void clearModels();
    void pushModel(const std::shared_ptr<Model> model);
    void setModels(const std::vector<std::shared_ptr<Model>>& models);

  private:
    void clean();
    void recreateSwapchain();

    std::shared_ptr<Window> m_window;

    // Destruction through RAII for following members:
    Instance m_instance;
    DebugMessenger m_debugMessenger;
    Surface m_surface;
    Device m_device;
    RenderPass m_renderPass;
    Swapchain m_swapchain;
    ImageViews m_imageViews;
    Framebuffers m_framebuffers;
    DescriptorSetLayout m_descriptorSetLayout;
    Pipeline m_pipeline;
    CommandPool m_graphicsCmdPool;
    DescriptorPool m_descriptorPool;
    std::vector<Fence> m_inFlightFences;
    std::vector<Semaphore> m_imageAvailableSemaphores;
    std::vector<Semaphore> m_finishedRenderSemaphores;
    DepthImage m_depthImage;
    CommandBuffers m_commandBuffers;
    std::vector<std::shared_ptr<Model>> m_models;

    Gui m_imgui;

    // No destruction necessary for following members:
    Images m_images;
    PhysicalDevice m_physicalDevice;
    Queues m_queues;
    std::vector<VkFence> m_imagesInFlight;
  };
}

#endif // API_HPP