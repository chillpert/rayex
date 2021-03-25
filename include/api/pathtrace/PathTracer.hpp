#pragma once

#include "api/pathtrace/AccelerationStructure.hpp"
#include "base/Geometry.hpp"
#include "base/Settings.hpp"

namespace RAYEX_NAMESPACE
{
  struct PtPushConstants
  {
    glm::vec4 clearColor        = glm::vec4( 1.0F );
    int frameCount              = 0;
    uint32_t sampleRatePerPixel = 4;
    uint32_t recursionDepth     = 4;
    uint32_t useEnvironmentMap  = 0;

    uint32_t russianRoulette               = 0;
    uint32_t russianRouletteMinBounces     = 0;
    uint32_t nextEventEstimation           = 0;
    uint32_t nextEventEstimationMinBounces = 0;
  };

  struct PathTracingCapabilities
  {
    vk::PhysicalDeviceRayTracingPipelinePropertiesKHR pipelineProperties; ///< The physical device's path tracing capabilities.
    vk::PhysicalDeviceAccelerationStructurePropertiesKHR accelerationStructureProperties;
    /*
    vk::PhysicalDeviceRayTracingPipelineFeaturesKHR pipelineFeatures;
    vk::PhysicalDeviceAccelerationStructureFeaturesKHR accelerationStructureFeatures;
    vk::PhysicalDeviceRayQueryFeaturesKHR rayQueryFeatures;
    */
  };

  /// Manages the building process of the acceleration structures.
  /// @ingroup API
  class PathTracer
  {
  public:
    PathTracer( ) = default;

    /// Calls destroy().
    ~PathTracer( );

    PathTracer( const PathTracer& )  = delete;
    PathTracer( const PathTracer&& ) = delete;

    auto operator=( const PathTracer& ) -> PathTracer& = delete;
    auto operator=( const PathTracer&& ) -> PathTracer& = delete;

    /// Retrieves the physical device's path tracing capabilities.
    void init( );

    /// Destroys all bottom and top level acceleration structures.
    void destroy( );

    /// @return Returns the top level acceleration structure.
    auto getTlas( ) const -> const Tlas& { return _tlas; }

    auto getStorageImageInfo( ) const -> const vk::DescriptorImageInfo& { return _storageImageInfo; }

    auto getCapabilities( ) const -> const PathTracingCapabilities& { return _capabilities; }

    auto getStorageImageView( ) const -> vk::ImageView { return _storageImageView.get( ); }

    auto getPipeline( ) const -> vk::Pipeline { return _pipeline.get( ); }

    auto getPipelineLayout( ) const -> vk::PipelineLayout { return _layout.get( ); };

    auto getDescriptorSetLayout( ) -> vk::DescriptorSetLayout { return _descriptors.layout.get( ); }

    auto getDescriptorSet( size_t index ) -> vk::DescriptorSet { return _descriptorSets[index]; }

    /// Used to convert wavefront models to a bottom level acceleration structure.
    /// @param vertexBuffer A vertex buffer of some geometry.
    /// @param indexBuffer An index buffer of some geometry.
    /// @return Returns the bottom level acceleration structure.
    auto modelToBlas( const vkCore::StorageBuffer<Vertex>& vertexBuffer, const vkCore::StorageBuffer<uint32_t>& indexBuffer, bool allowTransforms ) const -> Blas;

    /// Used to convert a bottom level acceleration structure instance to a Vulkan geometry instance.
    /// @param instance A bottom level acceleration structure instance.
    /// @return Returns the Vulkan geometry instance.
    auto geometryInstanceToAccelerationStructureInstance( std::shared_ptr<GeometryInstance>& geometryInstance ) -> vk::AccelerationStructureInstanceKHR;

    /// Used to prepare building the bottom level acceleration structures.
    /// @param vertexBuffers Vertex buffers of all geometry in the scene.
    /// @param indexBuffers Index buffers of all geometry in the scene.
    void createBottomLevelAS( std::vector<vkCore::StorageBuffer<Vertex>>& vertexBuffers, const std::vector<vkCore::StorageBuffer<uint32_t>>& indexBuffers, const std::vector<std::shared_ptr<Geometry>>& geometries );

    /// Builds all bottom level acceleration structures.
    /// @param blas_ A vector of RAYEX_NAMESPACE::Blas objects containing all bottom level acceleration structures prepared in createBottomLevelAS().
    /// @param flags The build flags.
    void buildBlas( vk::BuildAccelerationStructureFlagsKHR flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace );

    /// Build the top level acceleration structure.
    /// @param instances A vector of bottom level acceleration structure instances.
    /// @param flags The build flags.
    void buildTlas( const std::vector<std::shared_ptr<GeometryInstance>>& geometryInstances, vk::BuildAccelerationStructureFlagsKHR flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace, bool reuse = false );

    /// Creates the storage image which the path tracing shaders will write to.
    /// @param swapchainExtent The swapchain images' extent.
    void createStorageImage( vk::Extent2D swapchainExtent );

    /// Creates the shader binding tables.
    void createShaderBindingTable( );

    /// Used to create a path tracing pipeline.
    /// @param descriptorSetLayouts The descriptor set layouts for the shaders.
    /// @param settings The renderer settings.
    void createPipeline( const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts );

    /// Used to record the actual path tracing commands to a given command buffer.
    /// @param swapchaincommandBuffer The command buffer to record to.
    /// @param swapchainImage The current image in the swapchain.
    /// @param extent The swapchain images' extent.
    void pathTrace( vk::CommandBuffer swapchaincommandBuffer, vk::Image swapchainImage, vk::Extent2D extent );

    void initDescriptorSet( );

    void updateDescriptors( );

    void initVarianceBuffer( float width, float height );

    float getVariance( int width, int height, int perPixelSamples );

  private:
    vk::UniquePipeline _pipeline;
    vk::UniquePipelineLayout _layout;
    uint32_t _shaderGroups;
    PathTracingCapabilities _capabilities;
    std::vector<Blas> _blas_;
    Tlas _tlas; ///< The top level acceleration structure.
    vkCore::Buffer _instanceBuffer;
    vkCore::Buffer _sbtBuffer; ///< The shader binding table buffer.

    vkCore::Image _storageImage;
    vk::UniqueImageView _storageImageView;
    vk::UniqueSampler _storageImageSampler;
    vk::DescriptorImageInfo _storageImageInfo;

    vkCore::Descriptors _descriptors;
    std::vector<vk::DescriptorSet> _descriptorSets;

    vkCore::Buffer _varianceBuffer;
  };
} // namespace RAYEX_NAMESPACE
