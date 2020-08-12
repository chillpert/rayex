#ifndef RAYTRACING_BUILDER_HPP
#define RAYTRACING_BUILDER_HPP

#include "AccelerationStructure.hpp"
#include "Model.hpp"
#include "NodeBase.hpp"
#include "Buffer.hpp"
#include "Swapchain.hpp"

namespace rx
{
  class RayTracingBuilder
  {
  public:
    ~RayTracingBuilder( );

    void init( );
    void destroy( );

    inline const Tlas& getTlas( ) const { return m_tlas; }
    inline const vk::DescriptorSetLayout getDescriptorSetLayout( ) const { return m_descriptorSetLayout.get( ); }

    Blas objectToVkGeometryKHR( const std::shared_ptr<Model> model ) const;
    vk::AccelerationStructureInstanceKHR instanceToVkGeometryInstanceKHR( const BlasInstance& instance ) const;

    void createBottomLevelAS( const std::vector<std::shared_ptr<Model>>& models );
    void buildBlas( const std::vector<Blas>& blas_, vk::BuildAccelerationStructureFlagsKHR flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace );

    void createTopLevelAS( const std::vector<std::shared_ptr<GeometryNodeBase>>& nodes );
    void buildTlas( const std::vector<BlasInstance>& instances, vk::BuildAccelerationStructureFlagsKHR flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace );

    void createDescriptorSetLayout( const Swapchain& swapchain, uint32_t nodes );
    void createDescriptorSet( const Swapchain& swapchain );

    void createShaderBindingTable( vk::Pipeline rtPipeline );

  private:
    vk::PhysicalDeviceRayTracingPropertiesKHR m_rtProperties;

    std::vector<Blas> m_blas_;
    Tlas m_tlas;

    Buffer m_instBuffer;
  
    // Descriptors
    vk::UniqueDescriptorPool m_descriptorPool;
    DescriptorSet m_descriptorSets;
    DescriptorSetLayout m_descriptorSetLayout;

    // Storage image.
    Image m_storageImage;
    vk::UniqueImageView m_storageImageView;
  };
}

#endif // RAYTRACING_BUILDER_HPP