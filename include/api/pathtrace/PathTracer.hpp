#pragma once

#include "api/Swapchain.hpp"
#include "api/buffers/Buffer.hpp"
#include "api/buffers/IndexBuffer.hpp"
#include "api/buffers/VertexBuffer.hpp"
#include "api/pathtrace/AccelerationStructure.hpp"
#include "base/Geometry.hpp"
#include "base/Settings.hpp"

namespace RAYEX_NAMESPACE
{
  struct PtPushConstants
  {
    glm::vec4 clearColor           = glm::vec4( 1.0F );
    uint32_t frameCount            = 0;
    uint32_t sampleRatePerPixel    = 4;
    uint32_t recursionDepth        = 4;
    uint32_t directionalLightCount = 0;
    uint32_t pointLightCount       = 0;
    uint32_t useEnvironmentMap     = 0;

    uint32_t padding0 = 0;
    uint32_t padding1 = 0;
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

    auto getStorageImage( ) const -> const Image& { return _storageImage; }

    /// @return Returns the physical device's path tracing properties.
    auto getDevicePathTracingProperties( ) const -> const vk::PhysicalDeviceRayTracingPropertiesKHR& { return _ptProperties; }

    /// @return Returns the storage image's image view.
    auto getStorageImageView( ) const -> vk::ImageView { return _storageImageView.get( ); }

    /// @return Returns the path tracing pipeline.
    auto getPipeline( ) const -> vk::Pipeline { return _pipeline.get( ); }

    /// @return Returns the path tracing pipeline layout.
    auto getPipelineLayout( ) const -> vk::PipelineLayout { return _layout.get( ); };

    /// Used to convert wavefront models to a bottom level acceleration structure.
    /// @param vertexBuffer A vertex buffer of some geometry.
    /// @param indexBuffer An index buffer of some geometry.
    /// @return Returns the bottom level acceleration structure.
    auto modelToBlas( const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer, bool allowTransforms ) const -> Blas;

    /// Used to convert a bottom level acceleration structure instance to a Vulkan geometry instance.
    /// @param instance A bottom level acceleration structure instance.
    /// @return Returns the Vulkan geometry instance.
    auto instanceToVkGeometryInstanceKHR( const BlasInstance& instance ) -> vk::AccelerationStructureInstanceKHR;

    /// Used to prepare building the bottom level acceleration structures.
    /// @param vertexBuffers Vertex buffers of all geometry in the scene.
    /// @param indexBuffers Index buffers of all geometry in the scene.
    void createBottomLevelAS( const std::vector<VertexBuffer>& vertexBuffers, const std::vector<IndexBuffer>& indexBuffers, const std::vector<std::shared_ptr<Geometry>>& geometries );

    void updateDynamicBottomLevelAS( );

    /// Builds all bottom level acceleration structures.
    /// @param blas_ A vector of RAYEX_NAMESPACE::Blas objects containing all bottom level acceleration structures prepared in createBottomLevelAS().
    /// @param flags The build flags.
    void buildBlas( std::vector<Blas>& blas_, vk::BuildAccelerationStructureFlagsKHR flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace );

    /// Used to prepare building the top level acceleration structure.
    /// @param geometryInstances All geometry instances in the scene.
    void createTopLevelAS( const std::vector<std::shared_ptr<GeometryInstance>>& geometryInstances );

    /// Build the top level acceleration structure.
    /// @param instances A vector of bottom level acceleration structure instances.
    /// @param flags The build flags.
    void buildTlas( const std::vector<BlasInstance>& instances, vk::BuildAccelerationStructureFlagsKHR flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace, bool reuse = false );

    /// Creates the storage image which the path tracing shaders will write to.
    /// @param swapchainExtent The swapchain images' extent.
    void createStorageImage( vk::Extent2D swapchainExtent );

    /// Creates the shader binding tables.
    void createShaderBindingTable( );

    /// Used to create a path tracing pipeline.
    /// @param descriptorSetLayouts The descriptor set layouts for the shaders.
    /// @param settings The renderer settings.
    void createPipeline( const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts, Settings* settings );

    /// Used to record the actual path tracing commands to a given command buffer.
    /// @param swapchaincommandBuffer The command buffer to record to.
    /// @param swapchainImage The current image in the swapchain.
    /// @param extent The swapchain images' extent.
    void pathTrace( vk::CommandBuffer swapchaincommandBuffer, vk::Image swapchainImage, vk::Extent2D extent );

    std::vector<BlasInstance> instances;

  private:
    vk::UniquePipeline _pipeline;                            ///< The Vulkan pipeline with a unique handle.
    vk::UniquePipelineLayout _layout;                        ///< The Vulkan pipeline layout with a unique handle.
    uint32_t _shaderGroups;                                  ///< The total amount of path tracing shaders.
    vk::PhysicalDeviceRayTracingPropertiesKHR _ptProperties; ///< The physical device's path tracing capabilities.
    std::vector<Blas> _staticBlas_;                          ///< A vector containing all static bottom level acceleration structures.
    std::vector<Blas> _dynamicBlas_;                         ///< A vector containing all dynamic bottom level acceleration structures.
    std::vector<Blas> _blas_;                                ///< Both, all static and dynamic blas.
    std::map<uint32_t, std::pair<size_t, bool>> _indices;
    Tlas _tlas;                            ///< The top level acceleration structure.
    Buffer _instanceBuffer;                ///< The instance buffer.
    Buffer _sbtBuffer;                     ///< The shader binding table buffer.
    Image _storageImage;                   ///< The storage image.
    vk::UniqueImageView _storageImageView; ///< The storage image's image view.
  };
} // namespace RAYEX_NAMESPACE
