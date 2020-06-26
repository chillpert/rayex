#include "BottomLevelAS.hpp"
#include "QueryPool.hpp"
#include "CommandPool.hpp"
#include "CommandBuffer.hpp"
#include "Memory.hpp"

namespace RX
{
  vk::AccelerationStructureCreateGeometryTypeInfoKHR getGeometryTypeInfo(const std::shared_ptr<Model> model)
  {
    vk::AccelerationStructureCreateGeometryTypeInfoKHR typeInfo;
    typeInfo.pNext = nullptr;
    typeInfo.geometryType = vk::GeometryTypeKHR::eTriangles;
    typeInfo.maxPrimitiveCount = model->m_indexBuffer.getCount() / 3;
    typeInfo.indexType = model->m_indexBuffer.getType();
    typeInfo.maxVertexCount = model->m_vertexBuffer.getCount();
    typeInfo.vertexFormat = vk::Format::eR32G32B32Sfloat; // Note: Hard-coded value.
    typeInfo.allowsTransforms = VK_FALSE;

    return typeInfo;
  }

  BottomLevelAS::~BottomLevelAS()
  {
    if (m_as)
      destroy();
  }

  void BottomLevelAS::destroy()
  {
    m_info.device.destroyAccelerationStructureKHR(m_as, nullptr, m_info.dispatchLoaderDynamic);
    m_as = nullptr;

    m_info.device.freeMemory(m_memory);
    m_memory = nullptr;
  }

  void initBottomLevelAS_(BottomLevelASInfo& info, const std::vector<std::shared_ptr<Model>>& models, std::vector<BottomLevelAS>& blas_)
  {
    blas_.resize(models.size());

    std::vector<vk::AccelerationStructureCreateGeometryTypeInfoKHR> geometryInfos;
    geometryInfos.reserve(models.size());

    for (const auto& model : models)
    {
      geometryInfos.push_back(getGeometryTypeInfo(model));
    }

    vk::AccelerationStructureCreateInfoKHR createInfo;
    createInfo.pNext = nullptr;
    createInfo.compactedSize = 0; // TODO: Use compaction to improve performance.
    createInfo.type = vk::AccelerationStructureTypeKHR::eBottomLevel;
    createInfo.flags = info.flags;
    createInfo.maxGeometryCount = static_cast<uint32_t>(geometryInfos.size());
    createInfo.pGeometryInfos = geometryInfos.data();
    createInfo.deviceAddress = 0; // Only required if CaptureReplay feature is being used.

    for (auto& blas : blas_)
    {
      blas.m_info = info;

      // Create the acceleration structure.
      blas.m_as = info.device.createAccelerationStructureKHR(createInfo, nullptr, info.dispatchLoaderDynamic);
      if (!blas.m_as)
        RX_ERROR("Failed to create acceleration structure.");

      // Allocate memory.
      vk::AccelerationStructureMemoryRequirementsInfoKHR memReqInfo;
      memReqInfo.pNext = nullptr;
      memReqInfo.type = vk::AccelerationStructureMemoryRequirementsTypeKHR::eObject;
      memReqInfo.buildType = vk::AccelerationStructureBuildTypeKHR::eDevice; // TODO: check.
      memReqInfo.accelerationStructure = blas.m_as;

      vk::MemoryRequirements2 memReq2;
      info.device.getAccelerationStructureMemoryRequirementsKHR(&memReqInfo, &memReq2, info.dispatchLoaderDynamic);
    
      vk::MemoryAllocateInfo memAllocInfo;
      memAllocInfo.pNext = nullptr;
      memAllocInfo.allocationSize = memReq2.memoryRequirements.size;
      memAllocInfo.memoryTypeIndex = Memory::findType(info.physicalDevice, memReq2.memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
    
      blas.m_memory = info.device.allocateMemory(memAllocInfo);
      if (!blas.m_memory)
        RX_ERROR("Failed to allocate memory.");

      // Bind memory.
      vk::BindAccelerationStructureMemoryInfoKHR bindInfo;
      bindInfo.pNext = nullptr;
      bindInfo.accelerationStructure = blas.m_as;
      bindInfo.memory = blas.m_memory;
      bindInfo.memoryOffset = 0;
      bindInfo.deviceIndexCount = 0;
      bindInfo.pDeviceIndices = nullptr;

      vk::Result res = info.device.bindAccelerationStructureMemoryKHR(1, &bindInfo, info.dispatchLoaderDynamic);
      if (res != vk::Result::eSuccess)
        RX_ERROR("Failed to bind acceleration structure memory.");

      // Retrieve handle.
      vk::AccelerationStructureDeviceAddressInfoKHR addressInfo;
      addressInfo.pNext = nullptr;
      addressInfo.accelerationStructure = blas.m_as;

      blas.m_handle = info.device.getAccelerationStructureAddressKHR(addressInfo, info.dispatchLoaderDynamic);
    }
  }

  void BottomLevelAS::init(BottomLevelASInfo& info, const std::shared_ptr<Model> model, std::vector<BottomLevelAS>& blasCopy)
  {
    // TODO
    RX_ASSERT(false, "not implemented yet");
  }
}