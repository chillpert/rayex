#ifndef RAYTRACING_BUILDER_HPP
#define RAYTRACING_BUILDER_HPP

#include "AccelerationStructure.hpp"
#include "Model.hpp"

namespace rx
{
  class RayTracingBuilder
  {
  public:
    ~RayTracingBuilder( );

    void init( );
    void destroy( );

    void createBottomLevelAS( const std::vector<std::shared_ptr<Model>>& models );
    void buildBlas( const std::vector<Blas>& blas_, vk::BuildAccelerationStructureFlagsKHR flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace );

  private:
    vk::PhysicalDeviceRayTracingPropertiesKHR m_rtProperties;

    std::vector<Blas> m_blas_;
  };
}

#endif // RAYTRACING_BUILDER_HPP