#ifndef RAYTRACE_BUILDER_HPP
#define RAYTRACE_BUILDER_HPP

#include "pch/stdafx.hpp"
#include "Model.hpp"

namespace RX
{
  struct BottomLevelAS;

  struct RaytraceBuilderInfo
  {
    vk::Instance instance;
    vk::PhysicalDevice physicalDevice;
    vk::Device device;
    uint32_t queueFamilyIndex;
  };

  class RaytraceBuilder
  {
  public:

    inline RaytraceBuilderInfo& getInfo() { return m_info; }

    void initialize(RaytraceBuilderInfo& info);
    void destroy();

    BottomLevelAS objToBlas(const std::shared_ptr<Model> model);

    void createAllBLAS(const std::vector<std::shared_ptr<Model>> models);
    void buildAllBLAS(const std::vector<BottomLevelAS>& allBlas, vk::BuildAccelerationStructureFlagsKHR flags);

  private:
    RaytraceBuilderInfo m_info;

    VkPhysicalDeviceRayTracingPropertiesKHR m_rayTracingProperties;
    vk::DispatchLoaderDynamic m_dispatchLoaderDynamic;
  };

  struct BottomLevelAS
  {
    // VkAccelerationStructureKHR
    vk::AccelerationStructureKHR accelerationStructure; // NOTE: This is a naive implementation
    vk::DeviceMemory accelerationStructureAllocation;

    // specifying additional parameters for acceleration structure builds
    vk::BuildAccelerationStructureFlagsKHR flags;
    // specifies the shape of geometries that will be
    std::vector<vk::AccelerationStructureCreateGeometryTypeInfoKHR> asCreateGeometryInfo;
    // built into an acceleration structure
    std::vector<vk::AccelerationStructureGeometryKHR> asGeometry;
    // data used to build acceleration structure geometry
    std::vector<vk::AccelerationStructureBuildOffsetInfoKHR> asBuildOffsetInfo;
  };
}

#endif // RAYTRACE_BUILDER_HPP