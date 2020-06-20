#ifndef RAYTRACE_BUILDER_HPP
#define RAYTRACE_BUILDER_HPP

#include "pch/stdafx.hpp"
#include "Queue.hpp"
#include "DebugUtility.hpp"
#include "DescriptorPool.hpp"
#include "DescriptorSet.hpp"
#include "DescriptorSetLayout.hpp"
#include "TopLevelAS.hpp"
#include "BottomLevelAS.hpp"

namespace RX
{
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

    void initAccelerationStructures(const std::vector<std::shared_ptr<GeometryNodeBase>>& nodes, const std::vector<std::shared_ptr<Model>>& models);
    
  private:
    void initDescriptorSet();

    RaytraceBuilderInfo m_info;

    vk::PhysicalDeviceRayTracingPropertiesKHR m_rayTracingProperties;
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
}

#endif // RAYTRACE_BUILDER_HPP