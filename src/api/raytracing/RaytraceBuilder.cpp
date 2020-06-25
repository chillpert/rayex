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

    //m_debugUtils.initialize(DebugUtilInfo{ m_info.device, m_dispatchLoaderDynamic });
  }

  void RaytraceBuilder::destroy()
  {

  }

  void RaytraceBuilder::initAccelerationStructures(const std::vector<std::shared_ptr<GeometryNodeBase>>& nodes, const std::vector<std::shared_ptr<Model>>& models)
  {
    BottomLevelASInfo bottomLevelASInfo{ };
    bottomLevelASInfo.device = m_info.device;
    bottomLevelASInfo.dispatchLoaderDynamic = m_info.dispatchLoaderDynamic;
    bottomLevelASInfo.physicalDevice = m_info.physicalDevice;
    bottomLevelASInfo.flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace;
    bottomLevelASInfo.queue = m_info.queue;

    // Create all bottom level acceleration structures at once.
    initBottomLevelAS_(bottomLevelASInfo, models, m_blas);

    TopLevelASInfo topLevelASInfo{ };
    topLevelASInfo.device = m_info.device;
    topLevelASInfo.physicalDevice = m_info.physicalDevice;
    topLevelASInfo.dispatchLoaderDynamic = m_info.dispatchLoaderDynamic;
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
    DescriptorPoolInfo poolInfo{
      .device = m_info.device,
      .poolSizes = poolSizes,
      .maxSets = 1
    };

    m_descriptorPool.initialize(poolInfo);

    // Create raytracing shaders.
    m_rayGen.initialize(
      ShaderInfo{
        .fullPath = RX_SHADER_PATH "raygen.rgen",
        .device = m_info.device
      }
    );

    m_miss.initialize(
      ShaderInfo{
        .fullPath = RX_SHADER_PATH "miss.rmiss",
        .device = m_info.device
      }
    );

    m_closestHit.initialize(
      ShaderInfo{
        .fullPath = RX_SHADER_PATH "closesthit.rchit",
        .device = m_info.device
      }
    );

    // Init descriptor set layout.
    vk::DescriptorSetLayoutBinding asLayoutBinding{
      0,                                                                              // binding
      vk::DescriptorType::eAccelerationStructureKHR,                                  // descriptorType
      1,                                                                              // descriptorCount
      vk::ShaderStageFlagBits::eRaygenKHR | vk::ShaderStageFlagBits::eClosestHitKHR,  // stageFlags
      nullptr                                                                         // pImmutableSamplers
    };

    vk::DescriptorSetLayoutBinding storageImageLayoutBinding{
      1,                                    // binding
      vk::DescriptorType::eStorageImage,    // descriptorType
      1,                                    // descriptorCount
      vk::ShaderStageFlagBits::eRaygenKHR,  // stageFlags
      nullptr                               // pImmutableSamplers
    };

    m_descriptorSetLayout.addBinding(asLayoutBinding);
    m_descriptorSetLayout.addBinding(storageImageLayoutBinding);

    DescriptorSetLayoutInfo layoutInfo{
      .device = m_info.device,
    };

    m_descriptorSetLayout.initialize(layoutInfo);

    // Init descriptor set.
    DescriptorSetInfo setInfo{
      .device = m_info.device,
      .pool = m_descriptorPool.get(),
      .setCount = 1,
      .layouts = { m_descriptorSetLayout.get() }
    };
    
    m_descriptorSet.initialize(setInfo);
    
    // Create the storage image.
    ImageInfo imageInfo{ };
    imageInfo.physicalDevice = m_info.physicalDevice;
    imageInfo.device = m_info.device;
    imageInfo.usage = vk::ImageUsageFlagBits::eStorage;
    imageInfo.format = m_info.surface->getFormat();
    imageInfo.extent = vk::Extent3D(m_info.surface->getCapabilities().currentExtent, 1);

    m_storageImage.initialize(imageInfo);

    ImageViewInfo imageViewInfo{
      .device = m_info.device,
      .image = m_storageImage.get(),
      .format = m_storageImage.getFormat()
    };

    m_storageImageView.initialize(imageViewInfo);

    // Update descriptor set.
    UpdateRaytracingDescriptorSetInfo updateInfo{
      .tlas = m_tlas.get(),
      .storageImageView = m_storageImageView.get()
    };

    m_descriptorSet.update(updateInfo);
  }
}