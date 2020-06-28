#include "RaytraceBuilder.hpp"
#include "CommandPool.hpp"
#include "CommandBuffer.hpp"
#include "QueryPool.hpp"
#include "Buffer.hpp"
#include "Components.hpp"

namespace RX
{
  void RaytraceBuilder::init(RaytraceBuilderInfo& info)
  {
    m_info = info;

    auto properties = g_physicalDevice.getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceRayTracingPropertiesKHR>();
    m_rayTracingProperties = properties.get<vk::PhysicalDeviceRayTracingPropertiesKHR>();

    //m_debugUtils.init(DebugUtilInfo{ m_info.device, m_dispatchLoaderDynamic });
  }

  void RaytraceBuilder::destroy()
  {
    m_rayGen.destroy();
    m_miss.destroy();
    m_closestHit.destroy();

    m_tlas.destroy();
    for (auto& blas : m_blas)
      blas.destroy();

    m_storageImageView.destroy();
    m_storageImage.destroy();

    m_descriptorSet.destroy();
    m_descriptorSetLayout.destroy();
    m_descriptorPool.destroy();
  }

  void RaytraceBuilder::initAccelerationStructures(const std::vector<std::shared_ptr<GeometryNodeBase>>& nodes, const std::vector<std::shared_ptr<Model>>& models)
  {
    BottomLevelASInfo bottomLevelASInfo{ };
    bottomLevelASInfo.flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace;
    bottomLevelASInfo.queue = m_info.queue;

    // Create all bottom level acceleration structures at once.
    initBottomLevelAS_(bottomLevelASInfo, models, m_blas);

    TopLevelASInfo topLevelASInfo{ };
    topLevelASInfo.nodes = nodes;
    topLevelASInfo.flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace;
    
    // Create a single bottom level acceleration structure.
    m_tlas.init(topLevelASInfo);

    // Buffers
    std::vector<Buffer> buffers(nodes.size());
    
    struct RaytraceInstanceData
    {
      RaytracingInstance instance;
      glm::mat4 worldTransform;
    };

    BufferInfo info{
      .physicalDevice = g_physicalDevice,
      .device = g_device,
      .pNextBuffer = nullptr,
      .bufferFlags = { },
      .size = sizeof(RaytraceInstanceData),
      .usage = vk::BufferUsageFlagBits::eRayTracingKHR,
      .sharingMode = vk::SharingMode::eExclusive,
      .queueFamilyIndices = { },
      .commandPool = nullptr,
      .queue = nullptr,
      .memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
      .pNextMemory = nullptr,
      .memoryOffset = 0
    };

    vk::AccelerationStructureMemoryRequirementsInfoKHR memReqInfo{
      vk::AccelerationStructureMemoryRequirementsTypeKHR::eBuildScratch, // type
      { }, // buildType
      nullptr // accelerationStructure
    };

    vk::DeviceSize blasTotalMemorySize = 0;

    for (size_t i = 0; i < buffers.size(); ++i)
    {
      buffers[i].init(info);

      RaytraceInstanceData data{
        .instance = nodes[i]->m_rtInstance,
        .worldTransform = nodes[i]->m_worldTransform
      };

      buffers[i].fill(&data);

      memReqInfo.accelerationStructure = m_blas[i].get();


      vk::MemoryRequirements2 memReqBlas = g_device.getAccelerationStructureMemoryRequirementsKHR(memReqInfo, *g_dispatchLoaderDynamic);

      blasTotalMemorySize += memReqBlas.memoryRequirements.size;
    }

    vk::MemoryRequirements2 memReqTlas = g_device.getAccelerationStructureMemoryRequirementsKHR(memReqInfo, *g_dispatchLoaderDynamic);
    const vk::DeviceSize scratchBufferSize = std::max(blasTotalMemorySize, memReqTlas.memoryRequirements.size);

    BufferInfo scratchBufferInfo{
      .physicalDevice = g_physicalDevice,
      .device = g_device,
      .pNextBuffer = nullptr,
      .bufferFlags = { },
      .size = scratchBufferSize,
      .usage = vk::BufferUsageFlagBits::eRayTracingKHR,
      .sharingMode = vk::SharingMode::eExclusive,
      .queueFamilyIndices = { },
      .commandPool = nullptr,
      .queue = nullptr,
      .memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal,
      .pNextMemory = nullptr,
      .memoryOffset = 0
    };

    Buffer scratchBuffer(scratchBufferInfo);

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
    m_descriptorPool.init({
      .device = g_device,
      .poolSizes = poolSizes,
      .maxSets = 1,
      .flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet
      }
    );

    // Create raytracing shaders.
    m_rayGen.init({
        .fullPath = RX_SHADER_PATH "raygen.rgen",
        .device = g_device
      }
    );

    m_miss.init({
        .fullPath = RX_SHADER_PATH "miss.rmiss",
        .device = g_device
      }
    );

    m_closestHit.init({
        .fullPath = RX_SHADER_PATH "closesthit.rchit",
        .device = g_device
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

    m_descriptorSetLayout.init({ g_device });

    // Create the descriptor set.
    m_descriptorSet.init({
        .device = g_device,
        .pool = m_descriptorPool.get(),
        .setCount = 1,
        .layouts = { m_descriptorSetLayout.get() }
      }
    );

    // Create the storage image.
    ImageInfo imageInfo{ };
    imageInfo.physicalDevice = g_physicalDevice;
    imageInfo.device = g_device;
    imageInfo.usage = vk::ImageUsageFlagBits::eStorage;
    imageInfo.format = m_info.surface->getFormat();
    imageInfo.extent = vk::Extent3D(m_info.surface->getCapabilities().currentExtent, 1);

    m_storageImage.init(imageInfo);

    ImageViewInfo imageViewInfo{
      .device = g_device,
      .image = m_storageImage.get(),
      .format = m_storageImage.getFormat()
    };

    m_storageImageView.init(imageViewInfo);

    // Update descriptor set.
    UpdateRaytracingDescriptorSetInfo updateInfo{
      .tlas = m_tlas.get(),
      .storageImageView = m_storageImageView.get()
    };

    m_descriptorSet.update(updateInfo);
  }
}