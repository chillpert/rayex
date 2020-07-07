#ifndef RAYTRACING_BUILDER_HPP
#define RAYTRACING_BUILDER_HPP

#include "AccelerationStructure.hpp"
#include "Model.hpp"
#include "NodeBase.hpp"
#include "Buffer.hpp"

namespace rx
{
  class RayTracingBuilder
  {
  public:
    ~RayTracingBuilder( );

    void init( );
    void destroy( );

    Blas objectToVkGeometryKHR( const std::shared_ptr<Model> model ) const;
    vk::AccelerationStructureInstanceKHR instanceToVkGeometryInstanceKHR( const BlasInstance& instance ) const;

    void createBottomLevelAS( const std::vector<std::shared_ptr<Model>>& models );
    void buildBlas( const std::vector<Blas>& blas_, vk::BuildAccelerationStructureFlagsKHR flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace );

    void createTopLevelAS( const std::vector<std::shared_ptr<GeometryNodeBase>>& nodes );
    void buildTlas( const std::vector<BlasInstance>& instances, vk::BuildAccelerationStructureFlagsKHR flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace );

    void createDescriptorSet( );

  private:
    vk::PhysicalDeviceRayTracingPropertiesKHR m_rtProperties;

    std::vector<Blas> m_blas_;
    Tlas m_tlas;

    Buffer m_instBuffer;
  
    // Descriptors
    vk::UniqueDescriptorPool m_descriptorPool;
    DescriptorSet m_descriptorSets;
    DescriptorSetLayout m_descriptorSetLayout;
  };
}

#endif // RAYTRACING_BUILDER_HPP