#include "RaytraceBuilder.hpp"
#include "CommandPool.hpp"
#include "CommandBuffer.hpp"
#include "QueryPool.hpp"

namespace RX
{
  void RaytraceBuilder::initialize(RaytraceBuilderInfo& info)
  {
    m_info = info;

    auto properties = m_info.physicalDevice.getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceRayTracingPropertiesKHR>();
    m_rayTracingProperties = properties.get<vk::PhysicalDeviceRayTracingPropertiesKHR>();

    m_dispatchLoaderDynamic = vk::DispatchLoaderDynamic(m_info.instance, vkGetInstanceProcAddr);

    //m_debugUtils.initialize(DebugUtilInfo{ m_info.device, m_dispatchLoaderDynamic });
  }

  void RaytraceBuilder::destroy()
  {

  }

  void RaytraceBuilder::initAccelerationStructures(const std::vector<std::shared_ptr<GeometryNodeBase>>& nodes, const std::vector<std::shared_ptr<Model>>& models)
  {
    BottomLevelASInfo bottomLevelASInfo{ };
    bottomLevelASInfo.device = m_info.device;
    bottomLevelASInfo.dispatchLoaderDynamic = m_dispatchLoaderDynamic;
    bottomLevelASInfo.physicalDevice = m_info.physicalDevice;
    bottomLevelASInfo.flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace;
    bottomLevelASInfo.queue = m_info.queue;

    // Create all bottom level acceleration structures at once.
    initBottomLevelAS_(bottomLevelASInfo, models, m_blas);

    TopLevelASInfo topLevelASInfo{ };
    topLevelASInfo.device = m_info.device;
    topLevelASInfo.physicalDevice = m_info.physicalDevice;
    topLevelASInfo.dispatchLoaderDynamic = m_dispatchLoaderDynamic;
    topLevelASInfo.nodes = nodes;
    topLevelASInfo.flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace;
    
    // Create a single bottom level acceleration structure.
    m_tlas.initialize(topLevelASInfo);

    // Create descriptor set.
    initDescriptorSet();
  }

  void RaytraceBuilder::initDescriptorSet()
  {
    std::vector<vk::DescriptorPoolSize> poolSizes =
    {
      { vk::DescriptorType::eAccelerationStructureKHR, 1 },
      { vk::DescriptorType::eStorageImage, 1 },
      { vk::DescriptorType::eUniformBuffer, 1 },
      { vk::DescriptorType::eStorageBuffer, 2 }
    };

    // Init descriptor pool.
    DescriptorPoolInfo poolInfo{ };
    poolInfo.device = m_info.device;
    poolInfo.poolSizes = poolSizes;
    poolInfo.maxSets = 1;

    m_descriptorPool.initialize(poolInfo);

    // Init descriptor set layout.
    vk::DescriptorSetLayoutBinding asLayoutBinding;
    asLayoutBinding.binding = 0;
    asLayoutBinding.descriptorType = vk::DescriptorType::eAccelerationStructureKHR;
    asLayoutBinding.descriptorCount = 1;
    asLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eRaygenKHR | vk::ShaderStageFlagBits::eClosestHitKHR;
    asLayoutBinding.pImmutableSamplers = nullptr;

    vk::DescriptorSetLayoutBinding storageImageLayoutBinding;
    storageImageLayoutBinding.binding = 1;
    storageImageLayoutBinding.descriptorType = vk::DescriptorType::eStorageImage;
    storageImageLayoutBinding.descriptorCount = 1;
    storageImageLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eRaygenKHR;
    storageImageLayoutBinding.pImmutableSamplers = nullptr;

    DescriptorSetLayoutInfo layoutInfo{ };
    layoutInfo.device = m_info.device;
    layoutInfo.layoutBindings = { asLayoutBinding, storageImageLayoutBinding };

    m_descriptorSetLayout.initialize(layoutInfo);

    // Init descriptor set.
    DescriptorSetInfo setInfo{ };
    setInfo.device = m_info.device;
    setInfo.pool = m_descriptorPool.get();
    setInfo.setCount = 1;
    setInfo.layouts = { m_descriptorSetLayout.get() };
    
    m_descriptorSet.initialize(setInfo);
    
    // Storage Image
    {
      // Create the storage image.
      ImageInfo imageInfo{ };
      imageInfo.physicalDevice = m_info.physicalDevice;
      imageInfo.device = m_info.device;
      imageInfo.usage = vk::ImageUsageFlagBits::eStorage;
      imageInfo.format = m_info.surface->getFormat();
      imageInfo.extent = vk::Extent3D(m_info.surface->getCapabilities().currentExtent, 1);

      m_storageImage.initialize(imageInfo);

      ImageViewInfo imageViewInfo;
      imageViewInfo.device = m_info.device;
      imageViewInfo.image = m_storageImage.get();
      imageViewInfo.format = m_storageImage.getFormat();

      m_storageImageView.initialize(imageViewInfo);
    } 

    // Update descriptor set.
    UpdateRaytracingDescriptorSetInfo updateInfo{ };
    updateInfo.tlas = m_tlas.get();
    updateInfo.storageImageView = m_storageImageView.get(); // TODO: what to put here?

    m_descriptorSet.update(updateInfo);
  }
}