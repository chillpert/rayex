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
#include "Surface.hpp"

namespace RX
{
  struct RaytraceBuilderInfo
  {
    vk::Instance instance;
    vk::PhysicalDevice physicalDevice;
    vk::Device device;
    Surface* surface;
    std::shared_ptr<Queue> queue;
    vk::DispatchLoaderDynamic dispatchLoaderDynamic;
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
    inline Shader& getRayGenShader() { return m_rayGen; }
    inline Shader& getMissShader() { return m_miss; }
    inline Shader& getClosestHitShader() { return m_closestHit; }

    void init(RaytraceBuilderInfo& info);
    void destroy();

    void initAccelerationStructures(const std::vector<std::shared_ptr<GeometryNodeBase>>& nodes, const std::vector<std::shared_ptr<Model>>& models);
    
  private:
    void initDescriptorSet();

    RaytraceBuilderInfo m_info;

    vk::PhysicalDeviceRayTracingPropertiesKHR m_rayTracingProperties;

    std::vector<BottomLevelAS> m_blas;
    TopLevelAS m_tlas;

    std::vector<Instance> m_instances;

    Image m_storageImage;
    ImageView m_storageImageView;

    //DebugUtility m_debugUtil;

    DescriptorPool m_descriptorPool;
    DescriptorSetLayout m_descriptorSetLayout;
    DescriptorSet m_descriptorSet;

    Shader m_rayGen;
    Shader m_miss;
    Shader m_closestHit;
  };
}

#endif // RAYTRACE_BUILDER_HPP