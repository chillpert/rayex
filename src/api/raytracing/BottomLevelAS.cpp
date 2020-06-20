#include "BottomLevelAS.hpp"
#include "QueryPool.hpp"
#include "CommandPool.hpp"
#include "CommandBuffer.hpp"

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

  BottomLevelAS::BottomLevelAS(BottomLevelASInfo& info) :
    m_info(info) { }

  BottomLevelAS::~BottomLevelAS()
  {
    if (as)
      destroy();
  }

  void BottomLevelAS::destroy()
  {
    m_info.device.destroyAccelerationStructureKHR(as, nullptr, m_info.dispatchLoaderDynamic);
    as = nullptr;
  }

  void initBottomLevelAS_(BottomLevelASInfo& info, const std::unordered_map<std::string, std::shared_ptr<Model>>& models, std::vector<BottomLevelAS>& blas_)
  {
    vk::BuildAccelerationStructureFlagsKHR asFlags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace;

    blas_.resize(models.size());

    std::vector<vk::AccelerationStructureCreateGeometryTypeInfoKHR> geometryInfos;
    geometryInfos.reserve(models.size());

    for (const auto& model : models)
    {
      geometryInfos.push_back(getGeometryTypeInfo(model.second));
    }

    vk::AccelerationStructureCreateInfoKHR createInfo;
    createInfo.pNext = nullptr;
    createInfo.compactedSize = 0; // TODO: Use compaction to improve performance.
    createInfo.type = vk::AccelerationStructureTypeKHR::eBottomLevel;
    createInfo.flags = asFlags;
    createInfo.maxGeometryCount = static_cast<uint32_t>(geometryInfos.size());
    createInfo.pGeometryInfos = geometryInfos.data();
    createInfo.deviceAddress = 0; // Only required if CaptureReplay feature is being used.

    // Create acceleration structures.
    for (auto& blas : blas_)
    {
      blas.as = info.device.createAccelerationStructureKHR(createInfo, nullptr, info.dispatchLoaderDynamic);
      
      vk::AccelerationStructureMemoryRequirementsInfoKHR memReqInfo;
      memReqInfo.pNext = nullptr;
      memReqInfo.type = vk::AccelerationStructureMemoryRequirementsTypeKHR::eObject;
      memReqInfo.buildType = vk::AccelerationStructureBuildTypeKHR::eDevice; // TODO: check.
      memReqInfo.accelerationStructure = blas.as;

      vk::MemoryRequirements2 memReq2;
      info.device.getAccelerationStructureMemoryRequirementsKHR(&memReqInfo, &memReq2, info.dispatchLoaderDynamic);
    
      vk::MemoryAllocateInfo memAllocInfo;
      memAllocInfo.pNext = nullptr;
      memAllocInfo.allocationSize = memReq2.memoryRequirements.size;
      memAllocInfo.memoryTypeIndex;
    }

    

  }

  void BottomLevelAS::initialize(BottomLevelASInfo& info, const std::shared_ptr<Model> model, std::vector<BottomLevelAS>& blasCopy)
  {
    // TODO
    RX_ASSERT(false, "not implemented yet");
  }
}