#ifndef RAYTRACE_BUILDER_HPP
#define RAYTRACE_BUILDER_HPP

#include "pch/stdafx.hpp"
#include "Model.hpp"
#include "Queue.hpp"
#include "DebugUtility.hpp"
#include "DescriptorPool.hpp"
#include "DescriptorSet.hpp"
#include "DescriptorSetLayout.hpp"

namespace RX
{
  struct BottomLevelAS;
  // Top-level acceleration structure

  struct TopLevelAS
  {
    ~TopLevelAS();
    void destroy();

    vk::AccelerationStructureKHR accelerationStructure;
    vk::DeviceMemory accelerationStructureAllocation;
    vk::AccelerationStructureCreateInfoKHR asInfo;
    vk::BuildAccelerationStructureFlagsKHR flags;

    vk::DispatchLoaderDynamic dispatchLoaderDynamic;
    vk::Device device;
  };

  struct RaytraceBuilderInfo
  {
    vk::Instance instance;
    vk::PhysicalDevice physicalDevice;
    vk::Device device;
    std::shared_ptr<Queue> queue;
  };

  class RaytraceBuilder
  {
  private:
    struct Instance
    {
      glm::mat4 transform = glm::mat4(1.0f);
      uint32_t blasId = 0;      // Index of the BLAS in m_blas
      uint32_t instanceId = 0;  // Instance Index (gl_InstanceID)
      uint32_t hitGroupId = 0;  // Hit group index in the SBT
      uint32_t mask = 0xFF;     // Visibility mask, will be AND-ed with ray mask
      vk::GeometryInstanceFlagsKHR flags = vk::GeometryInstanceFlagBitsKHR::eTriangleFacingCullDisable;
    };

  public:
    inline RaytraceBuilderInfo& getInfo() { return m_info; }

    void initialize(RaytraceBuilderInfo& info);
    void destroy();

    BottomLevelAS objToBlas(const std::shared_ptr<Model> model);

    void createBottomLevelAS(const std::vector<std::shared_ptr<Model>> models);
    void createTopLevelAS(const std::vector<std::shared_ptr<Model>> models);

    void buildBottomLevelAS(const std::vector<BottomLevelAS>& allBlas, vk::BuildAccelerationStructureFlagsKHR flags);
    void buildTopLevelAS(const std::vector<Instance> allTlas, vk::BuildAccelerationStructureFlagsKHR flags);

    vk::AccelerationStructureInstanceKHR instanceToVkGeometryInstanceKHR(const Instance& instance);

    void createDescriptorSet();

  private:
    RaytraceBuilderInfo m_info;

    VkPhysicalDeviceRayTracingPropertiesKHR m_rayTracingProperties;
    vk::DispatchLoaderDynamic m_dispatchLoaderDynamic;

    std::vector<BottomLevelAS> m_blas;
    TopLevelAS m_tlas;

    std::vector<Instance> m_instances;

    //DebugUtility m_debugUtil;

    std::vector<vk::DescriptorSetLayoutBinding> m_bindings;
    std::vector<vk::DescriptorBindingFlags> m_bindingFlags;
    DescriptorPool m_descriptorPool;
    DescriptorSetLayout m_descriptorSetLayout;
    DescriptorSet m_descriptorSet;
  };

  struct BottomLevelAS
  {
    ~BottomLevelAS();
    void destroy();

    // This is bound to each model. But you have to find a solution so that models will be reused.
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
  
    vk::DispatchLoaderDynamic dispatchLoaderDynamic;
    vk::Device device;
  };
}

#endif // RAYTRACE_BUILDER_HPP