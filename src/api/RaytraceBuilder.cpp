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

    return blas;
  }

  void RaytraceBuilder::createAllBLAS(const std::vector<std::shared_ptr<Model>> models)
  {
    std::vector<BottomLevelAS> allBlas;
    allBlas.reserve(models.size());
    for (const auto& obj : models)
    {
      auto blas = objToBlas(obj);

      // We could add more geometry in each BLAS, but we add only one for now
      allBlas.emplace_back(blas);
    }

    buildAllBLAS(allBlas, vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace);
  }

  void RaytraceBuilder::buildAllBLAS(const std::vector<BottomLevelAS>& allBlas, vk::BuildAccelerationStructureFlagsKHR flags)
  {
    auto m_blas = allBlas;  // Keeping a copy

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
    CommandPoolInfo commandPoolInfo{ };
    commandPoolInfo.device = m_info.device;
    commandPoolInfo.queueFamilyIndex = m_info.queue->getIndex();

    CommandPool commandPool(commandPoolInfo);

    int ctr = 0;
    CommandBufferInfo commandBufferInfo{ };
    commandBufferInfo.device = m_info.device;
    commandBufferInfo.commandPool = commandPool.get();
    commandBufferInfo.queue = m_info.queue->get();

    /*
    commandBufferCount = 1; // Amount of command buffers that will be created.
    usageFlags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
    resetFlags = vk::CommandBufferResetFlagBits::eReleaseResources;
    level = vk::CommandBufferLevel::ePrimary;
    freeAutomatically = true;
    submitAutomatically = true; // Only submits automatically if usageFlags contains ONE_TIME_SUBMIT

    vk::CommandBufferBeginInfo beginInfo; // Ignore
    vk::SubmitInfo submitInfo; // Ignore

    std::vector<VkCommandBuffer> allCmdBufs;
    allCmdBufs.reserve(m_blas.size());
    for (auto& blas : m_blas)
    {
      VkCommandBuffer cmdBuf = genCmdBuf.createCommandBuffer();
      allCmdBufs.push_back(cmdBuf);

    }
    */
  }
}