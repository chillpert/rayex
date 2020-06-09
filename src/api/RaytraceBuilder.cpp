#include "RaytraceBuilder.hpp"

namespace RX
{
  void RaytraceBuilder::initialize(RaytraceBuilderInfo& info)
  {
    m_info = info;

    auto properties = m_info.physicalDevice.getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceRayTracingPropertiesKHR>();
    m_rayTracingProperties = properties.get<vk::PhysicalDeviceRayTracingPropertiesKHR>();
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
    int idx = 0;
    for (auto& blas : m_blas)
    {
      vk::AccelerationStructureCreateInfoKHR asCreateInfo;
      asCreateInfo.type = vk::AccelerationStructureTypeKHR::eBottomLevel;
      asCreateInfo.flags = flags;
      asCreateInfo.maxGeometryCount = static_cast<uint32_t>(blas.asCreateGeometryInfo.size());
      asCreateInfo.pGeometryInfos = blas.asCreateGeometryInfo.data();

      vk::DispatchLoaderDynamic instanceLoader(m_info.instance, vkGetInstanceProcAddr);
      blas.accelerationStructure = m_info.device.createAccelerationStructureKHR(asCreateInfo, nullptr, instanceLoader);
      //m_debug.setObjectName(blas.as.accel, (std::string("Blas" + std::to_string(idx)).c_str()));
      
      vk::AccelerationStructureMemoryRequirementsInfoKHR memoryRequirementsInfo;
      memoryRequirementsInfo.type = vk::AccelerationStructureMemoryRequirementsTypeKHR::eBuildScratch;
      memoryRequirementsInfo.accelerationStructure = blas.accelerationStructure;
      memoryRequirementsInfo.buildType = vk::AccelerationStructureBuildTypeKHR::eDevice;
      
      instanceLoader = vk::DispatchLoaderDynamic(m_info.instance, vkGetInstanceProcAddr);
      vk::MemoryRequirements2 reqMem = m_info.device.getAccelerationStructureMemoryRequirementsKHR(memoryRequirementsInfo, instanceLoader);
      vk::DeviceSize scratchSize = reqMem.memoryRequirements.size;
    }
  }
}