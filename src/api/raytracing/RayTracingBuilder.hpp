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
    inline const vk::ImageView getStorageImageView( ) const { return m_storageImageView.get( ); }

    Blas modelToBlas( const std::shared_ptr<Model> model ) const;
    vk::AccelerationStructureInstanceKHR instanceToVkGeometryInstanceKHR( const BlasInstance& instance );

    void createBottomLevelAS( const std::vector<std::shared_ptr<Model>>& models );
    void buildBlas( const std::vector<Blas>& blas_, vk::BuildAccelerationStructureFlagsKHR flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace );

    void createTopLevelAS( const std::vector<std::shared_ptr<GeometryNode>>& nodes );
    void buildTlas( const std::vector<BlasInstance>& instances, vk::BuildAccelerationStructureFlagsKHR flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace );

    void createStorageImage( vk::Extent2D swapchainExtent );
    void createShaderBindingTable( vk::Pipeline rtPipeline );

    void rayTrace( vk::CommandBuffer swapchaincommandBuffer, vk::Image swapchainImage, vk::Extent2D extent );

  private:
    vk::PhysicalDeviceRayTracingPropertiesKHR m_rtProperties;

    std::vector<Blas> m_blas_;
    Tlas m_tlas;

    Buffer m_instBuffer;
  
    Buffer m_sbtBuffer;

    // Storage image.
    Image m_storageImage;
    vk::UniqueImageView m_storageImageView;
  };
}

#endif // RAYTRACING_BUILDER_HPP