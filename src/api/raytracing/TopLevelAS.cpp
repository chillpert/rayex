#include "TopLevelAS.hpp"
#include "Memory.hpp"

namespace RX
{
  TopLevelAS::~TopLevelAS()
  {
    destroy();
  }

  void TopLevelAS::destroy()
  {
    if (m_as)
      m_info.device.destroyAccelerationStructureKHR(m_as, nullptr, m_info.dispatchLoaderDynamic);
    
    m_as = nullptr;

    if (m_memory)
      m_info.device.freeMemory(m_memory);

    m_memory = nullptr;
  }

  void TopLevelAS::initialize(TopLevelASInfo& info)
  {
    m_info = info;

    // Create top level acceleration structure.
    vk::AccelerationStructureCreateGeometryTypeInfoKHR typeInfo;
    typeInfo.pNext = nullptr;
    typeInfo.geometryType = vk::GeometryTypeKHR::eInstances;
    typeInfo.maxPrimitiveCount = static_cast<uint32_t>(m_info.nodes.size());
    typeInfo.indexType = vk::IndexType::eNoneKHR;
    typeInfo.maxVertexCount = 0;
    typeInfo.vertexFormat = vk::Format::eUndefined;
    typeInfo.allowsTransforms = VK_TRUE;

    vk::AccelerationStructureCreateInfoKHR createInfo;
    createInfo.pNext = nullptr;
    createInfo.compactedSize = 0; // TODO: Use compaction to improve performance.
    createInfo.type = vk::AccelerationStructureTypeKHR::eTopLevel;
    createInfo.flags = info.flags;
    createInfo.maxGeometryCount = 1;
    createInfo.pGeometryInfos = &typeInfo;
    createInfo.deviceAddress = 0; // Only required if CaptureReplay feature is being used.

    m_as = m_info.device.createAccelerationStructureKHR(createInfo, nullptr, m_info.dispatchLoaderDynamic);
    if (!m_as)
      RX_ERROR("Failed to create top level acceleration structure.");

    // Allocate memory.
    vk::AccelerationStructureMemoryRequirementsInfoKHR memReqInfo;
    memReqInfo.pNext = nullptr;
    memReqInfo.type = vk::AccelerationStructureMemoryRequirementsTypeKHR::eObject;
    memReqInfo.buildType = vk::AccelerationStructureBuildTypeKHR::eDevice;
    memReqInfo.accelerationStructure = m_as;

    vk::MemoryRequirements2 memReq2;
    m_info.device.getAccelerationStructureMemoryRequirementsKHR(&memReqInfo, &memReq2, m_info.dispatchLoaderDynamic);

    vk::MemoryAllocateInfo memAllocInfo;
    memAllocInfo.pNext = nullptr;
    memAllocInfo.allocationSize = memReq2.memoryRequirements.size;
    memAllocInfo.memoryTypeIndex = Memory::findType(m_info.physicalDevice, memReq2.memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

    m_memory = info.device.allocateMemory(memAllocInfo);
    if (!m_memory)
      RX_ERROR("Failed to allocate memory.");

    // Bind memory.
    vk::BindAccelerationStructureMemoryInfoKHR bindInfo;
    bindInfo.pNext = nullptr;
    bindInfo.accelerationStructure = m_as;
    bindInfo.memory = m_memory;
    bindInfo.memoryOffset = 0;
    bindInfo.deviceIndexCount = 0;
    bindInfo.pDeviceIndices = nullptr;

    vk::Result res = m_info.device.bindAccelerationStructureMemoryKHR(1, &bindInfo, m_info.dispatchLoaderDynamic);
    if (res != vk::Result::eSuccess)
      RX_ERROR("Failed to bind acceleration structure memory.");

    // Retrieve handle.
    vk::AccelerationStructureDeviceAddressInfoKHR addressInfo;
    addressInfo.pNext = nullptr;
    addressInfo.accelerationStructure = m_as;

    m_handle = info.device.getAccelerationStructureAddressKHR(addressInfo, info.dispatchLoaderDynamic);
  }
}