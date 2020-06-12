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

  BottomLevelAS RaytraceBuilder::objToBlas(const std::shared_ptr<Model> model)
  {
    vk::AccelerationStructureCreateGeometryTypeInfoKHR createInfo;
    createInfo.setGeometryType(vk::GeometryTypeKHR::eTriangles);
    createInfo.setIndexType(vk::IndexType::eUint32);
    createInfo.setVertexFormat(vk::Format::eR32G32B32Sfloat);
    createInfo.setMaxPrimitiveCount(model->m_indexBuffer.getCount() / 3);  // Nb triangles
    createInfo.setMaxVertexCount(model->m_vertexBuffer.getCount());
    createInfo.setAllowsTransforms(VK_FALSE); // No adding transformation matrices

    // Building part
    vk::BufferDeviceAddressInfo vertexAddressInfo;
    vertexAddressInfo.buffer = model->m_vertexBuffer.get();

    vk::DeviceAddress vertexAddress = m_info.device.getBufferAddress(vertexAddressInfo);
    vk::DeviceAddress indexAddress = m_info.device.getBufferAddress({ model->m_indexBuffer.get() });

    vk::AccelerationStructureGeometryTrianglesDataKHR triangles;
    triangles.setVertexFormat(createInfo.vertexFormat);
    triangles.setVertexData(vertexAddress);
    triangles.setVertexStride(sizeof(Vertex));
    triangles.setIndexType(createInfo.indexType);
    triangles.setIndexData(indexAddress);
    triangles.setTransformData({ });

    // Setting up the build info of the acceleration
    vk::AccelerationStructureGeometryKHR asGeom;
    asGeom.setGeometryType(createInfo.geometryType);
    asGeom.setFlags(vk::GeometryFlagBitsKHR::eOpaque);
    asGeom.geometry.setTriangles(triangles);

    // The primitive itself
    vk::AccelerationStructureBuildOffsetInfoKHR offset;
    offset.setFirstVertex(0);
    offset.setPrimitiveCount(createInfo.maxPrimitiveCount);
    offset.setPrimitiveOffset(0);
    offset.setTransformOffset(0);

    // Our blas is only one geometry, but could be made of many geometries
    BottomLevelAS blas;
    blas.asGeometry.emplace_back(asGeom);
    blas.asCreateGeometryInfo.emplace_back(createInfo);
    blas.asBuildOffsetInfo.emplace_back(offset);
    blas.device = m_info.device;
    blas.dispatchLoaderDynamic = m_dispatchLoaderDynamic;

    return blas;
  }

  void RaytraceBuilder::createBottomLevelAS(const std::vector<std::shared_ptr<Model>> models)
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

  void RaytraceBuilder::createTopLevelAS(const std::vector<std::shared_ptr<Model>> models)
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

  void RaytraceBuilder::buildBottomLevelAS(const std::vector<BottomLevelAS>& allBlas, vk::BuildAccelerationStructureFlagsKHR flags)
  {
    m_blas = allBlas;  // Keeping a copy

    vk::DeviceSize maxScratch = 0;  // Largest scratch buffer for our BLAS
    
    // Is compaction requested?
    bool doCompaction = (flags & vk::BuildAccelerationStructureFlagBitsKHR::eAllowCompaction) == vk::BuildAccelerationStructureFlagBitsKHR::eAllowCompaction;

    std::vector<vk::DeviceSize> originalSizes;
    originalSizes.resize(m_blas.size());

    // Iterate over the groups of geometries, creating one BLAS for each group
    size_t idx = 0;
    for (auto& blas : m_blas)
    {
      vk::AccelerationStructureCreateInfoKHR asCreateInfo;
      asCreateInfo.type = vk::AccelerationStructureTypeKHR::eBottomLevel;
      asCreateInfo.flags = flags;
      asCreateInfo.maxGeometryCount = static_cast<uint32_t>(blas.asCreateGeometryInfo.size());
      asCreateInfo.pGeometryInfos = blas.asCreateGeometryInfo.data();
      
      blas.accelerationStructure = m_info.device.createAccelerationStructureKHR(asCreateInfo, nullptr, m_dispatchLoaderDynamic);
      //m_debugUtils.setObjectName((uint64_t)(VkAccelerationStructureKHR(blas.accelerationStructure)), (std::string("Blas" + std::to_string(idx)).c_str()), vk::ObjectType::eAccelerationStructureKHR);

      vk::AccelerationStructureMemoryRequirementsInfoKHR memoryRequirementsInfo;
      memoryRequirementsInfo.type = vk::AccelerationStructureMemoryRequirementsTypeKHR::eBuildScratch;
      memoryRequirementsInfo.accelerationStructure = blas.accelerationStructure;
      memoryRequirementsInfo.buildType = vk::AccelerationStructureBuildTypeKHR::eDevice;
      
      vk::MemoryRequirements2 reqMem = m_info.device.getAccelerationStructureMemoryRequirementsKHR(memoryRequirementsInfo, m_dispatchLoaderDynamic);
      vk::DeviceSize scratchSize = reqMem.memoryRequirements.size;

      blas.flags = flags;
      maxScratch = std::max(maxScratch, scratchSize);

      memoryRequirementsInfo.type = vk::AccelerationStructureMemoryRequirementsTypeKHR::eObject;
      reqMem = m_info.device.getAccelerationStructureMemoryRequirementsKHR(memoryRequirementsInfo, m_dispatchLoaderDynamic);
      originalSizes[idx] = reqMem.memoryRequirements.size;

      ++idx;
    }

    // Allocate the scratch buffers holding the temporary data of the acceleration structure builder
    BufferCreateInfo bufferCreateInfo{ };
    bufferCreateInfo.physicalDevice = m_info.physicalDevice;
    bufferCreateInfo.device = m_info.device;
    bufferCreateInfo.size = maxScratch;
    bufferCreateInfo.usage = vk::BufferUsageFlagBits::eRayTracingKHR | vk::BufferUsageFlagBits::eShaderDeviceAddress;
    bufferCreateInfo.sharingMode = vk::SharingMode::eConcurrent;

    vk::MemoryAllocateFlagsInfo allocateFlags(vk::MemoryAllocateFlagBitsKHR::eDeviceAddress);
    bufferCreateInfo.pNextMemory = &allocateFlags;

    Buffer scratchBuffer(bufferCreateInfo);

    vk::BufferDeviceAddressInfo bufferInfo;
    bufferInfo.buffer = scratchBuffer.get();
    vk::DeviceAddress scratchAddress = m_info.device.getBufferAddress(bufferInfo, m_dispatchLoaderDynamic);

    // Query size of compact BLAS    
    QueryPoolInfo queryPoolInfo{ };
    queryPoolInfo.device = m_info.device;
    queryPoolInfo.dispatchLoaderDynamic = m_dispatchLoaderDynamic;
    queryPoolInfo.queryCount = static_cast<uint32_t>(m_blas.size());
    queryPoolInfo.queryType = vk::QueryType::eAccelerationStructureCompactedSizeKHR;

    QueryPool queryPool(queryPoolInfo);

    // Create a command buffer containing all the BLAS builds
    CommandPool commandPool(CommandPoolInfo{ m_info.device, m_info.queue->getIndex(), vk::CommandPoolCreateFlagBits::eResetCommandBuffer });

    int ctr = 0;
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
    
    for (size_t i = 0; i < m_blas.size(); ++i)
    {
      const vk::AccelerationStructureGeometryKHR* pGeometry = m_blas[i].asGeometry.data();
      
      vk::AccelerationStructureBuildGeometryInfoKHR bottomASInfo;
      bottomASInfo.type = vk::AccelerationStructureTypeKHR::eBottomLevel;
      bottomASInfo.flags = flags;
      bottomASInfo.update = VK_FALSE;
      bottomASInfo.srcAccelerationStructure = nullptr;
      bottomASInfo.dstAccelerationStructure = m_blas[i].accelerationStructure;
      bottomASInfo.geometryArrayOfPointers = VK_FALSE;
      bottomASInfo.geometryCount = static_cast<uint32_t>(m_blas[i].asGeometry.size());
      bottomASInfo.ppGeometries = &pGeometry;
      bottomASInfo.scratchData.deviceAddress = scratchAddress;

      // Pointers of offset
      std::vector<const vk::AccelerationStructureBuildOffsetInfoKHR*> pBuildOffset(m_blas[i].asBuildOffsetInfo.size());
      for (size_t j = 0; j < m_blas[i].asBuildOffsetInfo.size(); ++j)
        pBuildOffset[j] = &m_blas[i].asBuildOffsetInfo[j];

      commandBuffers.reset();
      commandBuffers.begin(i);

      // Building the AS
      commandBuffers.get(i).buildAccelerationStructureKHR(1, &bottomASInfo, pBuildOffset.data(), m_dispatchLoaderDynamic); // CMD

      // Since the scratch buffer is reused across builds, we need a barrier to ensure one build
      // is finished before starting the next one
      vk::MemoryBarrier barrier;
      barrier.srcAccessMask = vk::AccessFlagBits::eAccelerationStructureWriteKHR;
      barrier.dstAccessMask = vk::AccessFlagBits::eAccelerationStructureReadKHR;

      commandBuffers.get(i).pipelineBarrier(
        vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR, 
        vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR, 
        vk::DependencyFlagBits::eByRegion,
        1,
        &barrier,
        0,
        nullptr,
        0, 
        nullptr
      ); // CMD
      
      // Query the compact size
      if (doCompaction)
        commandBuffers.get(i).writeAccelerationStructuresPropertiesKHR(1, &m_blas[i].accelerationStructure, vk::QueryType::eAccelerationStructureCompactedSizeKHR, queryPool.get(), ++ctr, m_dispatchLoaderDynamic);
      
      commandBuffers.end(i);
    }

  }

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

    BufferCreateInfo bufferInfo{ };
    bufferInfo.physicalDevice = m_info.physicalDevice;
    bufferInfo.device = m_info.device;
    bufferInfo.size = scratchSize;
    bufferInfo.usage = vk::BufferUsageFlagBits::eRayTracingKHR | vk::BufferUsageFlagBits::eShaderDeviceAddress;
    bufferInfo.sharingMode = vk::SharingMode::eConcurrent;

    vk::MemoryAllocateFlagsInfo allocateFlags(vk::MemoryAllocateFlagBitsKHR::eDeviceAddress);
    bufferInfo.pNextMemory = &allocateFlags;
    vk::DeviceSize memoryOffset = 0;

    Buffer scratchBuffer(bufferInfo);

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
  }

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

  BottomLevelAS::~BottomLevelAS()
  {
    if (accelerationStructure)
      destroy();
  }

  void BottomLevelAS::destroy()
  {
    device.destroyAccelerationStructureKHR(accelerationStructure, nullptr, dispatchLoaderDynamic);
  }

  TopLevelAS::~TopLevelAS()
  {
    if (accelerationStructure)
      destroy();
  }

  void TopLevelAS::destroy()
  {
    device.destroyAccelerationStructureKHR(accelerationStructure, nullptr, dispatchLoaderDynamic);
  }
}