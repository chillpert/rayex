#ifndef RAYTRACE_BUILDER_HPP
#define RAYTRACE_BUILDER_HPP

#include "pch/stdafx.hpp"
#include "DescriptorSet.hpp"
#include "DescriptorSetLayout.hpp"
#include "TopLevelAS.hpp"
#include "BottomLevelAS.hpp"
#include "Surface.hpp"

namespace rx
{
  class RaytraceBuilder
  {
  public:
    inline vk::ShaderModule getRayGenShader( ) { return m_rayGen.get( ); }
    inline vk::ShaderModule getMissShader( ) { return m_miss.get( ); }
    inline vk::ShaderModule getClosestHitShader( ) { return m_closestHit.get( ); }

    void init( );
    void destroy( );

    void initAccelerationStructures( const std::vector<std::shared_ptr<GeometryNodeBase>>& nodes, const std::vector<std::shared_ptr<Model>>& models, const Surface* const surface );

  private:
    void initDescriptorSet( const Surface* const surface );

    vk::PhysicalDeviceRayTracingPropertiesKHR m_rayTracingProperties;

    std::vector<BottomLevelAS> m_blas;
    TopLevelAS m_tlas;

    Image m_storageImage;
    vk::UniqueImageView m_storageImageView;

    vk::UniqueDescriptorPool m_descriptorPool;
    DescriptorSetLayout m_descriptorSetLayout;
    DescriptorSet m_descriptorSet;

    vk::UniqueShaderModule m_rayGen;
    vk::UniqueShaderModule m_miss;
    vk::UniqueShaderModule m_closestHit;
  };
}

#endif // RAYTRACE_BUILDER_HPP