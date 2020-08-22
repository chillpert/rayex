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
    inline const DescriptorSet& getDescriptorSets( ) const { return m_descriptorSets; }

    Blas objectToVkGeometryKHR( const std::shared_ptr<Model> model ) const;
    vk::AccelerationStructureInstanceKHR instanceToVkGeometryInstanceKHR( const BlasInstance& instance ) const;

    void createBottomLevelAS( const std::vector<std::shared_ptr<Model>>& models );
    void buildBlas( const std::vector<Blas>& blas_, vk::BuildAccelerationStructureFlagsKHR flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace );

    void createTopLevelAS( const std::vector<std::shared_ptr<GeometryNode>>& nodes );
    void buildTlas( const std::vector<BlasInstance>& instances, vk::BuildAccelerationStructureFlagsKHR flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace );

    void createDescriptorPool( uint32_t swapchainImagesCount );
    void createDescriptorSetLayout( const Swapchain& swapchain, uint32_t nodes );
    void createDescriptorSets( uint32_t swapchainImagesCount );
    void updateDescriptorSets( const std::vector<vk::Buffer>& uniformBuffers );

    void createStorageImage( vk::Extent2D swapchainExtent );

    void createShaderBindingTable( vk::Pipeline rtPipeline );

    void rayTrace( vk::CommandBuffer swapchaincommandBuffer, vk::Image swapchainImage, vk::Extent2D extent );

    struct PushConstant
    {
      glm::vec4 clearColor;
      glm::vec3 lightPosition;
      float lightIntensity;
      int lightType;
    } m_rtPushConstants;

  private:
    vk::PhysicalDeviceRayTracingPropertiesKHR m_rtProperties;

    std::vector<Blas> m_blas_;
    Tlas m_tlas;

    Buffer m_instBuffer;
  
    Buffer m_sbtBuffer;

    // Descriptors
    vk::UniqueDescriptorPool m_descriptorPool;
    std::vector<vk::DescriptorSetLayoutBinding> m_bindings;
    DescriptorSet m_descriptorSets;
    DescriptorSetLayout m_descriptorSetLayout;

    // Storage image.
    Image m_storageImage;
    vk::UniqueImageView m_storageImageView;
  };
}

#endif // RAYTRACING_BUILDER_HPP