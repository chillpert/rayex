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
    // Init pool.
    m_bindings = { 
      { 0, vk::DescriptorType::eAccelerationStructureKHR, 1, vk::ShaderStageFlagBits::eRaygenKHR | vk::ShaderStageFlagBits::eClosestHitKHR },
      { 1, vk::DescriptorType::eStorageImage, 1, vk::ShaderStageFlagBits::eRaygenKHR }
    };

    DescriptorPoolInfo poolInfo{ };
    poolInfo.device = m_info.device;
    poolInfo.poolSizes = { { vk::DescriptorType::eAccelerationStructureKHR, 1 }, { vk::DescriptorType::eStorageImage, 1 } };
    poolInfo.maxSets = 1;

    m_descriptorPool.initialize(poolInfo);

    // Init descriptor set layout.
    DescriptorSetLayoutInfo layoutInfo{ };
    layoutInfo.device = m_info.device;
    layoutInfo.layoutBindings = m_bindings;

    m_descriptorSetLayout.initialize(layoutInfo);

    // Init descriptor set.
    DescriptorSetInfo setInfo{ };
    setInfo.device = m_info.device;
    setInfo.pool = m_descriptorPool.get();
    setInfo.layouts = { m_descriptorSetLayout.get() };

    m_descriptorSet.initialize(setInfo);

    // Create the storage image.
    ImageInfo imageInfo{ };
    imageInfo.physicalDevice = m_info.physicalDevice;
    imageInfo.device = m_info.device;
    imageInfo.usage = vk::ImageUsageFlagBits::eStorage;
    imageInfo.format = vk::Format::eR8G8B8A8Unorm;
    imageInfo.extent = vk::Extent3D(900, 600, 1);

    m_storageImage.initialize(imageInfo);

    ImageViewInfo imageViewInfo;
    imageViewInfo.device = m_info.device;
    imageViewInfo.image = m_storageImage.get();
    imageViewInfo.format = m_storageImage.getInfo().format;

    m_storageImageView.initialize(imageViewInfo);

    // Update descriptor set.
    UpdateRaytracingDescriptorSetInfo updateInfo{ };
    updateInfo.tlas = m_tlas.get();
    updateInfo.storageImageView = m_storageImageView.get(); // TODO: what to put here?
    updateInfo.bindings = m_bindings;

    m_descriptorSet.update(updateInfo);
  }
}