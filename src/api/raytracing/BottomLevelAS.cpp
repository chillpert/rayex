#include "BottomLevelAS.hpp"
#include "QueryPool.hpp"
#include "CommandPool.hpp"
#include "CommandBuffer.hpp"

namespace RX
{
  BottomLevelAS::BottomLevelAS(BottomLevelASInfo& info) :
    m_info(info) { }

  BottomLevelAS::~BottomLevelAS()
  {
    if (accelerationStructure)
      destroy();
  }

  void BottomLevelAS::destroy()
  {
    m_info.device.destroyAccelerationStructureKHR(accelerationStructure, nullptr, m_info.dispatchLoaderDynamic);
    accelerationStructure = nullptr;
  }

  void initBottomLevelAS_(BottomLevelASInfo& info, const std::vector<std::shared_ptr<Model>>& models)
  {
    std::vector<BottomLevelAS> blas_;
    blas_.reserve(models.size());
    for (const std::shared_ptr<Model> model : models)
    {
      // Convert model to bottom level acceleration structure.
      BottomLevelAS blas(info);
      blas = model;

      // We could add more geometry in each BLAS, but we add only one for now
      blas_.emplace_back(blas);
    }

    // Begin building the acceleration structures.
    auto m_blas = blas_;  // Keeping a copy

    vk::DeviceSize maxScratch = 0;  // Largest scratch buffer for our BLAS
    
    // Is compaction requested?
    bool doCompaction = (info.flags & vk::BuildAccelerationStructureFlagBitsKHR::eAllowCompaction) == vk::BuildAccelerationStructureFlagBitsKHR::eAllowCompaction;

    std::vector<vk::DeviceSize> originalSizes;
    originalSizes.resize(m_blas.size());

    // Iterate over the groups of geometries, creating one BLAS for each group
    size_t idx = 0;
    for (auto& blas : m_blas)
    {
      vk::AccelerationStructureCreateInfoKHR asCreateInfo;
      asCreateInfo.type = vk::AccelerationStructureTypeKHR::eBottomLevel;
      asCreateInfo.flags = info.flags;
      asCreateInfo.maxGeometryCount = static_cast<uint32_t>(blas.asCreateGeometryInfo.size());
      asCreateInfo.pGeometryInfos = blas.asCreateGeometryInfo.data();
      
      blas.accelerationStructure = info.device.createAccelerationStructureKHR(asCreateInfo, nullptr, info.dispatchLoaderDynamic);
      //m_debugUtils.setObjectName((uint64_t)(VkAccelerationStructureKHR(blas.accelerationStructure)), (std::string("Blas" + std::to_string(idx)).c_str()), vk::ObjectType::eAccelerationStructureKHR);

      vk::AccelerationStructureMemoryRequirementsInfoKHR memoryRequirementsInfo;
      memoryRequirementsInfo.type = vk::AccelerationStructureMemoryRequirementsTypeKHR::eBuildScratch;
      memoryRequirementsInfo.accelerationStructure = blas.accelerationStructure;
      memoryRequirementsInfo.buildType = vk::AccelerationStructureBuildTypeKHR::eDevice;
      
      vk::MemoryRequirements2 reqMem = info.device.getAccelerationStructureMemoryRequirementsKHR(memoryRequirementsInfo, info.dispatchLoaderDynamic);
      vk::DeviceSize scratchSize = reqMem.memoryRequirements.size;

      maxScratch = std::max(maxScratch, scratchSize);

      memoryRequirementsInfo.type = vk::AccelerationStructureMemoryRequirementsTypeKHR::eObject;
      reqMem = info.device.getAccelerationStructureMemoryRequirementsKHR(memoryRequirementsInfo, info.dispatchLoaderDynamic);
      originalSizes[idx] = reqMem.memoryRequirements.size;

      ++idx;
    }

    // Allocate the scratch buffers holding the temporary data of the acceleration structure builder
    BufferCreateInfo bufferCreateInfo{ };
    bufferCreateInfo.physicalDevice = info.physicalDevice;
    bufferCreateInfo.device = info.device;
    bufferCreateInfo.size = maxScratch;
    bufferCreateInfo.usage = vk::BufferUsageFlagBits::eRayTracingKHR | vk::BufferUsageFlagBits::eShaderDeviceAddress;
    bufferCreateInfo.sharingMode = vk::SharingMode::eConcurrent;

    vk::MemoryAllocateFlagsInfo allocateFlags2(vk::MemoryAllocateFlagBitsKHR::eDeviceAddress);
    bufferCreateInfo.pNextMemory = &allocateFlags2;

    Buffer scratchBuffer(bufferCreateInfo);

    vk::BufferDeviceAddressInfo bufferInfo;
    bufferInfo.buffer = scratchBuffer.get();
    vk::DeviceAddress scratchAddress = info.device.getBufferAddress(bufferInfo, info.dispatchLoaderDynamic);

    // Query size of compact BLAS    
    QueryPoolInfo queryPoolInfo{ };
    queryPoolInfo.device = info.device;
    queryPoolInfo.dispatchLoaderDynamic = info.dispatchLoaderDynamic;
    queryPoolInfo.queryCount = static_cast<uint32_t>(m_blas.size());
    queryPoolInfo.queryType = vk::QueryType::eAccelerationStructureCompactedSizeKHR;

    QueryPool queryPool(queryPoolInfo);

    // Create a command buffer containing all the BLAS builds
    CommandPool commandPool(CommandPoolInfo{ info.device, info.queue->getIndex(), vk::CommandPoolCreateFlagBits::eResetCommandBuffer });

    int ctr = 0;
    CommandBufferInfo commandBufferInfo{ };
    commandBufferInfo.device = info.device;
    commandBufferInfo.commandPool = commandPool.get();
    commandBufferInfo.queue = info.queue->get();
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
      bottomASInfo.flags = info.flags;
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
      commandBuffers.get(i).buildAccelerationStructureKHR(1, &bottomASInfo, pBuildOffset.data(), info.dispatchLoaderDynamic); // CMD

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
        commandBuffers.get(i).writeAccelerationStructuresPropertiesKHR(1, &m_blas[i].accelerationStructure, vk::QueryType::eAccelerationStructureCompactedSizeKHR, queryPool.get(), ++ctr, info.dispatchLoaderDynamic);
      
      commandBuffers.end(i);
    }
  }

  BottomLevelAS& BottomLevelAS::operator=(const std::shared_ptr<Model> model)
  {
    wavefrontToBottomLevelAS(model);
    return *this;
  }

  void BottomLevelAS::wavefrontToBottomLevelAS(std::shared_ptr<Model> model)
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
    asGeometry.emplace_back(asGeom);
    asCreateGeometryInfo.emplace_back(createInfo);
    asBuildOffsetInfo.emplace_back(offset);
  }
}