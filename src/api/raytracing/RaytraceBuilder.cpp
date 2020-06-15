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

  void RaytraceBuilder::initAccelerationStructures(const std::unordered_set<std::shared_ptr<ModelBase>> models)
  {
    //initBottomLevelAS(models);
    //initTopLevelAS(models);
    //initDescriptorSet();

    BottomLevelASInfo bottomLevelASInfo{ };
    bottomLevelASInfo.dispatchLoaderDynamic = m_dispatchLoaderDynamic;
    bottomLevelASInfo.device = m_info.device;
    bottomLevelASInfo.physicalDevice = m_info.physicalDevice;
    bottomLevelASInfo.flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace;
    bottomLevelASInfo.queue = m_info.queue;

    initBottomLevelAS_(bottomLevelASInfo, models);
  }

  void RaytraceBuilder::initDescriptorSet()
  {
    m_bindings = { 
      { 0, vk::DescriptorType::eAccelerationStructureKHR, 1, vk::ShaderStageFlagBits::eRaygenKHR | vk::ShaderStageFlagBits::eClosestHitKHR },
      { 1, vk::DescriptorType::eStorageImage, 1, vk::ShaderStageFlagBits::eRaygenKHR }
    };

    DescriptorPoolInfo poolInfo{ };
    poolInfo.device = m_info.device;
    poolInfo.poolSizes = { { vk::DescriptorType::eAccelerationStructureKHR, 1 }, { vk::DescriptorType::eStorageImage, 1 } };
    poolInfo.maxSets = 2;

    m_descriptorPool.initialize(poolInfo);

    DescriptorSetLayoutInfo layoutInfo{ };
    layoutInfo.device = m_info.device;
    layoutInfo.layoutBindings = m_bindings;

    m_descriptorSetLayout.initialize(layoutInfo);

    DescriptorSetInfo setInfo{ };
    setInfo.device = m_info.device;
    setInfo.pool = m_descriptorPool.get();
    setInfo.layouts = std::vector<vk::DescriptorSetLayout>(1, m_descriptorSetLayout.get());

    m_descriptorSet.initialize(setInfo);
  }

/*
  void RaytraceBuilder::initBottomLevelAS(const std::vector<std::shared_ptr<Model>> models)
  {
    std::vector<BottomLevelAS> allBlas;
    allBlas.reserve(models.size());
    for (const auto& model : models)
    {
      auto blas = objToBlas(model);

      // We could add more geometry in each BLAS, but we add only one for now
      allBlas.emplace_back(blas);
    }

    buildBottomLevelAS(allBlas, vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace);
  }
*/

/*
  void RaytraceBuilder::initTopLevelAS(const std::vector<std::shared_ptr<Model>> models)
  {
    std::vector<Instance> allTlas;
    allTlas.reserve(models.size());

    for (uint32_t i = 0; i < static_cast<int>(models.size()); ++i)
    {
      Instance rayInst;
      rayInst.transform = models[i]->m_model; // Position of the instance
      rayInst.instanceId = i; // gl_InstanceID
      rayInst.blasId = models[i]->m_objIndex;
      rayInst.hitGroupId = 0; // We will use the same hit group for all objects
      rayInst.flags = vk::GeometryInstanceFlagBitsKHR::eTriangleFacingCullDisable;
      allTlas.push_back(rayInst);
    }

    buildTopLevelAS(allTlas, vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace);
  }
*/

/*
  void RaytraceBuilder::buildTopLevelAS(const std::vector<Instance> instances, vk::BuildAccelerationStructureFlagsKHR flags)
  {
    m_tlas.flags = flags;
    m_tlas.device = m_info.device;
    m_tlas.dispatchLoaderDynamic = m_dispatchLoaderDynamic;

    vk::AccelerationStructureCreateGeometryTypeInfoKHR geometryCreate;
    geometryCreate.geometryType = vk::GeometryTypeKHR::eInstances;
    geometryCreate.maxPrimitiveCount = static_cast<uint32_t>(instances.size());
    geometryCreate.allowsTransforms = VK_TRUE;

    vk::AccelerationStructureCreateInfoKHR asCreateInfo;
    asCreateInfo.type = vk::AccelerationStructureTypeKHR::eTopLevel;
    asCreateInfo.flags = flags;
    asCreateInfo.maxGeometryCount = 1;
    asCreateInfo.pGeometryInfos = &geometryCreate;
    
    m_tlas.asInfo = asCreateInfo;
    m_tlas.accelerationStructure = m_info.device.createAccelerationStructureKHR(asCreateInfo, nullptr, m_dispatchLoaderDynamic);

    vk::AccelerationStructureMemoryRequirementsInfoKHR memoryRequirementsInfo;
    memoryRequirementsInfo.type = vk::AccelerationStructureMemoryRequirementsTypeKHR::eBuildScratch;
    memoryRequirementsInfo.accelerationStructure = m_tlas.accelerationStructure;
    memoryRequirementsInfo.buildType = vk::AccelerationStructureBuildTypeKHR::eDevice;

    vk::MemoryRequirements2 reqMem = m_info.device.getAccelerationStructureMemoryRequirementsKHR(memoryRequirementsInfo, m_dispatchLoaderDynamic);
    vk::DeviceSize scratchSize = reqMem.memoryRequirements.size;

    BufferCreateInfo scratchBufferInfo{ };
    scratchBufferInfo.physicalDevice = m_info.physicalDevice;
    scratchBufferInfo.device = m_info.device;
    scratchBufferInfo.size = scratchSize;
    scratchBufferInfo.usage = vk::BufferUsageFlagBits::eRayTracingKHR | vk::BufferUsageFlagBits::eShaderDeviceAddress;
    scratchBufferInfo.sharingMode = vk::SharingMode::eConcurrent;

    vk::MemoryAllocateFlagsInfo allocateFlags3(vk::MemoryAllocateFlagBitsKHR::eDeviceAddress);
    scratchBufferInfo.pNextMemory = &allocateFlags3;
    vk::DeviceSize memoryOffset = 0;

    Buffer scratchBuffer(scratchBufferInfo);

    vk::BufferDeviceAddressInfo bufferAddressInfo;
    bufferAddressInfo.buffer = scratchBuffer.get();
    vk::DeviceAddress scratchAddress = m_info.device.getBufferAddress(bufferAddressInfo);

    // For each instance, build the corresponding instance descriptor
    std::vector<vk::AccelerationStructureInstanceKHR> geometryInstances;
    geometryInstances.reserve(instances.size());
    for (const auto& inst : instances)
    {
      geometryInstances.push_back(instanceToVkGeometryInstanceKHR(inst));
    }

    // Building the TLAS
    CommandPool commandPool(CommandPoolInfo{ m_info.device, m_info.queue->getIndex(), vk::CommandPoolCreateFlagBits::eResetCommandBuffer });

    CommandBufferInfo commandBufferInfo{ };
    commandBufferInfo.device = m_info.device;
    commandBufferInfo.commandPool = commandPool.get();
    commandBufferInfo.queue = m_info.queue->get();
    commandBufferInfo.commandBufferCount = m_blas.size();
    commandBufferInfo.usageFlags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;
    commandBufferInfo.level = vk::CommandBufferLevel::ePrimary;
    commandBufferInfo.freeAutomatically = true;
    commandBufferInfo.submitAutomatically = true;

    CommandBuffer commandBuffers(commandBufferInfo);
    
    // Create a buffer holding the actual instance data for use by the AS builder
    vk::DeviceSize instanceDescsSizeInBytes = instances.size() * sizeof(vk::AccelerationStructureInstanceKHR);

    // Allocate the instance buffer and copy its contents from host to device
    // memory
    //m_instBuffer = m_alloc.createBuffer(commandBuffers.get(), geometryInstances,
    //  VK_BUFFER_USAGE_RAY_TRACING_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT);

    BufferCreateInfo instBufferInfo{ };
    instBufferInfo.physicalDevice = m_info.physicalDevice;
    instBufferInfo.device = m_info.device;
    instBufferInfo.size = scratchSize;
    instBufferInfo.usage = vk::BufferUsageFlagBits::eRayTracingKHR | vk::BufferUsageFlagBits::eShaderDeviceAddress;
    instBufferInfo.sharingMode = vk::SharingMode::eConcurrent;

    vk::MemoryAllocateFlagsInfo allocateFlags(vk::MemoryAllocateFlagBitsKHR::eDeviceAddress);
    instBufferInfo.pNextMemory = &allocateFlags;
    memoryOffset = 0;

    Buffer instBuffer(instBufferInfo);

    bufferAddressInfo.buffer = instBuffer.get();
    vk::DeviceAddress instanceAddress = m_info.device.getBufferAddress(bufferAddressInfo);
    
    // Make sure the copy of the instance buffer are copied before triggering the
    // acceleration structure build
    vk::MemoryBarrier barrier;
    barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
    barrier.dstAccessMask = vk::AccessFlagBits::eAccelerationStructureWriteKHR;

    commandBuffers.begin(0);

    commandBuffers.get(0).pipelineBarrier(
      vk::PipelineStageFlagBits::eTransfer,
      vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR,
      vk::DependencyFlagBits::eByRegion,
      1,
      &barrier,
      0,
      nullptr,
      0,
      nullptr
    ); // CMD

    commandBuffers.end(0);

    // Build the TLAS
    vk::AccelerationStructureGeometryDataKHR geometry;
    geometry.instances.arrayOfPointers = VK_FALSE;
    geometry.instances.data.deviceAddress = instanceAddress;
    vk::AccelerationStructureGeometryKHR topASGeometry;
    topASGeometry.geometryType = vk::GeometryTypeKHR::eInstances;
    topASGeometry.geometry = geometry;

    const vk::AccelerationStructureGeometryKHR* pGeometry = &topASGeometry;
    vk::AccelerationStructureBuildGeometryInfoKHR topASInfo;
    topASInfo.flags = flags;
    topASInfo.update = VK_FALSE;
    topASInfo.srcAccelerationStructure = nullptr;
    topASInfo.dstAccelerationStructure = m_tlas.accelerationStructure;
    topASInfo.geometryArrayOfPointers = VK_FALSE;
    topASInfo.geometryCount = 1;
    topASInfo.ppGeometries = &pGeometry;
    topASInfo.scratchData.deviceAddress = scratchAddress;

    // Build Offsets info: n instances
    vk::AccelerationStructureBuildOffsetInfoKHR buildOffsetInfo{ static_cast<uint32_t>(instances.size()), 0, 0, 0 };
    const vk::AccelerationStructureBuildOffsetInfoKHR* pBuildOffsetInfo = &buildOffsetInfo;

    // Build the TLAS
    commandBuffers.get(0).buildAccelerationStructureKHR(1, &topASInfo, &pBuildOffsetInfo, m_dispatchLoaderDynamic); // CMD
  }
*/

/*
  vk::AccelerationStructureInstanceKHR RaytraceBuilder::instanceToVkGeometryInstanceKHR(const Instance& instance)
  {
    BottomLevelAS& blas{ m_blas[instance.blasId] };

    vk::AccelerationStructureDeviceAddressInfoKHR addressInfo(blas.accelerationStructure);
    vk::DeviceAddress blasAddress = m_info.device.getAccelerationStructureAddressKHR(addressInfo, m_dispatchLoaderDynamic);

    vk::AccelerationStructureInstanceKHR gInst;
    // The matrices for the instance transforms are row-major, instead of
    // column-major in the rest of the application
    glm::mat4 transpose = glm::transpose(instance.transform);
    // The gInst.transform value only contains 12 values, corresponding to a 4x3
    // matrix, hence saving the last row that is anyway always (0,0,0,1). Since
    // the matrix is row-major, we simply copy the first 12 values of the
    // original 4x4 matrix
    memcpy(&gInst.transform, &transpose, sizeof(gInst.transform));
    gInst.instanceCustomIndex = instance.instanceId;
    gInst.mask = instance.mask;
    gInst.instanceShaderBindingTableRecordOffset = instance.hitGroupId;
    gInst.flags = VkGeometryFlagsKHR(instance.flags);
    gInst.accelerationStructureReference = blasAddress;

    return gInst;
  }
*/
}