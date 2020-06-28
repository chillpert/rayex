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
    Surface* surface;
    std::shared_ptr<Queue> queue;
    vk::CommandPool commandPool;
  };

  class RaytraceBuilder
  {
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

    Image m_storageImage;
    ImageView m_storageImageView;

    DescriptorPool m_descriptorPool;
    DescriptorSetLayout m_descriptorSetLayout;
    DescriptorSet m_descriptorSet;

    Shader m_rayGen;
    Shader m_miss;
    Shader m_closestHit;
  };
}

#endif // RAYTRACE_BUILDER_HPP