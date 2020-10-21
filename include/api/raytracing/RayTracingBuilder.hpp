#ifndef RAYTRACING_BUILDER_HPP
#define RAYTRACING_BUILDER_HPP

#include "api/Swapchain.hpp"
#include "api/buffers/Buffer.hpp"
#include "api/buffers/IndexBuffer.hpp"
#include "api/buffers/VertexBuffer.hpp"
#include "api/raytracing/AccelerationStructure.hpp"
#include "base/Geometry.hpp"

namespace RAYEX_NAMESPACE
{
  /// Manages the building process of the acceleration structures.
  /// @ingroup API
  class RayTracingBuilder
  {
  public:
    RayTracingBuilder( ) = default;

    /// Calls destroy().
    ~RayTracingBuilder( );

    RayTracingBuilder( const RayTracingBuilder& )  = delete;
    RayTracingBuilder( const RayTracingBuilder&& ) = delete;

    auto operator=( const RayTracingBuilder& ) -> RayTracingBuilder& = delete;
    auto operator=( const RayTracingBuilder && ) -> RayTracingBuilder& = delete;

    /// Retrieves the physical device's ray tracing capabilities.
    void init( );

    /// Destroys all bottom and top level acceleration structures.
    void destroy( );

    /// @return Returns the top level acceleration structure.
    auto getTlas( ) const -> const Tlas& { return _tlas; }

    /// @return Returns the physical device's ray tracing properties.
    auto getRtProperties( ) const -> const vk::PhysicalDeviceRayTracingPropertiesKHR& { return _rtProperties; }

    /// @return Returns the storage image's image view.
    auto getStorageImageView( ) const -> vk::ImageView { return _storageImageView.get( ); }

    /// Used to convert wavefront models to a bottom level acceleration structure.
    /// @param vertexBuffer A vertex buffer of some geometry.
    /// @param indexBuffer An index buffer of some geometry.
    /// @return Returns the bottom level acceleration structure.
    auto modelToBlas( const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer ) const -> Blas;

    /// Used to convert a bottom level acceleration structure instance to a Vulkan geometry instance.
    /// @param instance A bottom level acceleration structure instance.
    /// @return Returns the Vulkan geometry instance.
    auto instanceToVkGeometryInstanceKHR( const BlasInstance& instance ) -> vk::AccelerationStructureInstanceKHR;

    /// Used to prepare building the bottom level acceleration structures.
    /// @param vertexBuffers Vertex buffers of all geometry in the scene.
    /// @param indexBuffers Index buffers of all geometry in the scene.
    void createBottomLevelAS( const std::vector<VertexBuffer>& vertexBuffers, const std::vector<IndexBuffer>& indexBuffers );

    /// Builds all bottom level acceleration structures.
    /// @param blas_ A vector of RAYEX_NAMESPACE::Blas objects containing all bottom level acceleration structures prepared in createBottomLevelAS().
    /// @param flags The build flags.
    void buildBlas( const std::vector<Blas>& blas_, vk::BuildAccelerationStructureFlagsKHR flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace );

    /// Used to prepare building the top level acceleration structure.
    /// @param geometryInstances All geometry instances in the scene.
    void createTopLevelAS( const std::vector<std::shared_ptr<GeometryInstance>>& geometryInstances );

    /// Build the top level acceleration structure.
    /// @param instances A vector of bottom level acceleration structure instances.
    /// @param flags The build flags.
    void buildTlas( const std::vector<BlasInstance>& instances, vk::BuildAccelerationStructureFlagsKHR flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace );

    /// Creates the storage image which the ray tracing shaders will write to.
    /// @param swapchainExtent The swapchain images' extent.
    void createStorageImage( vk::Extent2D swapchainExtent );

    /// Creates the shader binding tables.
    /// @param rtPipeline The ray tracing graphics pipeline.
    void createShaderBindingTable( vk::Pipeline rtPipeline );

    /// Used to record the actual ray tracing commands to a given command buffer.
    /// @param swapchaincommandBuffer The command buffer to record to.
    /// @param swapchainImage The current image in the swapchain.
    /// @param extent The swapchain images' extent.
    void rayTrace( vk::CommandBuffer swapchaincommandBuffer, vk::Image swapchainImage, vk::Extent2D extent );

  private:
    vk::PhysicalDeviceRayTracingPropertiesKHR _rtProperties; ///< The physical device's ray tracing capabilities.

    std::vector<Blas> _blas_; ///< A vector containing all bottom level acceleration structures.
    Tlas _tlas;               ///< The top level acceleration structure.

    Buffer _instanceBuffer; ///< The instance buffer.

    Buffer _sbtBuffer; ///< The shader binding table buffer.

    Image _storageImage;                   ///< The storage image.
    vk::UniqueImageView _storageImageView; ///< The storage image's image view.
  };
} // namespace RAYEX_NAMESPACE

#endif // RAYTRACING_BUILDER_HPP
