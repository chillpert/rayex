#ifndef INITIALIZERS_HPP
#define INITIALIZERS_HPP

#include "api/misc/Components.hpp"
#include "api/raytracing/AccelerationStructure.hpp"

namespace vk
{
  /// A collection of functions to initialize or allocate Vulkan resources.
  /// @ingroup API
  namespace Initializer
  {
    /// Creates a fence with a unique handle.
    /// @param flags The flags used for creating the fence.
    /// @return Returns the fence.
    UniqueFence initFenceUnique( FenceCreateFlags flags = FenceCreateFlagBits::eSignaled );

    /// Creates a fence.
    /// @param flags The flags used for creating the fence.
    /// @return Returns the fence.
    Fence initFence( FenceCreateFlags flags = FenceCreateFlagBits::eSignaled );

    /// Creates a semaphore with a unique handle.
    /// @param flags The flags used for creating the semaphore.
    /// @return Returns the semaphore.
    UniqueSemaphore initSemaphoreUnique( SemaphoreCreateFlags flags = { } );

    /// Creates a semaphore.
    /// @param flags The flags used for creating the semaphore.
    /// @return Returns the semaphore.
    Semaphore initSemaphore( SemaphoreCreateFlags flags = { } );

    /// Creates a command pool with a unique handle.
    /// @param queueFamilyIndex The queue family from which the command pool can submit to.
    /// @param flags The flags used for creating the command pool.
    /// @return Returns the command pool.
    UniqueCommandPool initCommandPoolUnique( uint32_t queueFamilyIndex, CommandPoolCreateFlags flags = { } );

    /// Creates a command pool.
    /// @param queueFamilyIndex The queue family from which the command pool can submit to.
    /// @param flags The flags used for creating the command pool.
    /// @return Returns the command pool.
    CommandPool initCommandPool( uint32_t queueFamilyIndex, CommandPoolCreateFlags flags = { } );

    UniqueDescriptorPool initDescriptorPoolUnique( const std::vector<DescriptorPoolSize>& poolSizes, uint32_t maxSets = 1, DescriptorPoolCreateFlags flags = { } );

    /// Allocates and binds unique memory for an image.
    /// @param image The image to allocate memory for.
    /// @param pNext The pNext chain of the Vulkan allocation info.
    /// @return Returns the memory.
    UniqueDeviceMemory allocateMemoryUnique( Image image, MemoryPropertyFlags propertyFlags = MemoryPropertyFlagBits::eDeviceLocal, void* pNext = nullptr );

    /// Allocates and binds memory for an image.
    /// @param image The image to allocate memory for.
    /// @param pNext The pNext chain of the Vulkan allocation info.
    /// @return Returns the memory.
    DeviceMemory allocateMemory( Image image, MemoryPropertyFlags propertyFlags = MemoryPropertyFlagBits::eDeviceLocal, void* pNext = nullptr );

    /// Allocates and binds unique memory for a buffer.
    /// @param image The image to allocate memory for.
    /// @param pNext The pNext chain of the Vulkan allocation info.
    /// @return Returns the memory.
    UniqueDeviceMemory allocateMemoryUnique( Buffer buffer, MemoryPropertyFlags propertyFlags = MemoryPropertyFlagBits::eDeviceLocal, void* pNext = nullptr );

    /// Allocates and binds unique memory for an acceleration structure object.
    /// @param as The RAYEXEC_NAMESPACE::AccelerationStructure object the memory will be allocated for.
    void allocateMemory( RAYEXEC_NAMESPACE::AccelerationStructure& as );

    /// Allocates and binds memory for a buffer.
    /// @param image The image to allocate memory for.
    /// @param pNext The pNext chain of the Vulkan allocation info.
    /// @return Returns the memory.
    DeviceMemory allocateMemory( Buffer buffer, MemoryPropertyFlags propertyFlags = MemoryPropertyFlagBits::eDeviceLocal, void* pNext = nullptr );

    /// Create an image view with a unique handle.
    /// @param image The image to init an image view for.
    /// @param format The target format of the image view.
    /// @return Returns the image view.
    UniqueImageView initImageViewUnique( Image image, Format format, ImageAspectFlags aspectFlags = ImageAspectFlagBits::eColor );

    /// Create an image view.
    /// @param image The image to init an image view for.
    /// @param format The target format of the image view.
    /// @return Returns the image view.
    ImageView initImageView( Image image, Format format, ImageAspectFlags aspectFlags = ImageAspectFlagBits::eColor );

    /// Create a sampler with a unique handle.
    /// @param initInfo The Vulkan init info for the sampler.
    /// @return Returns the sampler.
    /// Note: Use RAYEXEC_NAMESPACE::Helper::getSamplerCreateInfo to get a predefined init info.
    UniqueSampler initSamplerUnique( const SamplerCreateInfo& initInfo );

    /// Create a sampler.
    /// @param initInfo The Vulkan init info for the sampler.
    /// @return Returns the sampler.
    /// Note: Use RAYEXEC_NAMESPACE::Helper::getSamplerCreateInfo to get a predefined init info.
    Sampler initSampler( const SamplerCreateInfo& initInfo );

    /// Create a framebuffer with a unique handle.
    /// @param attachments All the image view attachments for the framebuffer.
    /// @param renderPass The render pass for which the framebuffer will be used.
    /// @param extent The extent of the framebuffer.
    /// @return Returns the framebuffer.
    UniqueFramebuffer initFramebufferUnique( const std::vector<ImageView>& attachments, RenderPass renderPass, const Extent2D& extent );

    /// Create a framebuffer.
    /// @param attachments All the image view attachments for the framebuffer.
    /// @param renderPass The render pass for which the framebuffer will be used.
    /// @param extent The extent of the framebuffer.
    /// @return Returns the framebuffer.
    Framebuffer initFramebuffer( const std::vector<ImageView>& attachments, RenderPass renderPass, const Extent2D& extent );

    /// Create a query pool with a unique handle.
    /// @param count The number of queries managed by the pool.
    /// @param type Specifies the type of queries managed by the pool.
    /// return Returns the query pool.
    UniqueQueryPool initQueryPoolUnique( uint32_t count, QueryType type );

    /// Create a query pool.
    /// @param count The number of queries managed by the pool.
    /// @param type Specifies the type of queries managed by the pool.
    /// return Returns the query pool.
    QueryPool initQueryPool( uint32_t count, QueryType type );

    /// Parses a given shader and inits a shader module with a unique handle.
    /// @param path The full path to the GLSL shader file.
    /// @return Returns the shader module.
    UniqueShaderModule initShaderModuleUnique( std::string_view path );

    /// Parses a given shader and inits a shader module.
    /// @param path The full path to the GLSL shader file.
    /// @return Returns the shader module.
    ShaderModule initShaderModule( std::string_view path );

    /// Creates the acceleration structure and allocates and binds memory for it.
    /// @param asCreateInfo The Vulkan init info for the acceleration structure.
    /// @return Returns an RAYEXEC_NAMESPACE::AccelerationStructure object that contains the AS itself as well as the memory for it.
    RAYEXEC_NAMESPACE::AccelerationStructure initAccelerationStructure( const AccelerationStructureCreateInfoKHR& asCreateInfo );

    /// Retrieves the most suited GPU on the current machine.
    ///
    /// If a device is found its properties and features will be retrieved.
    /// @return Returns true if the initilization was successful.
    /// @note Multiple GPUs can not be utilized.
    PhysicalDevice initPhysicalDevice( );

    /// Retrieves the queue family indices for both graphics and transfer queue families.
    /// @note The function tries to find a transfer queue family index that is not the same as the graphics queue family index.
    /// @see hasDedicatedTransferQueueFamily(vk::PhysicalDevice)
    void initQueueFamilyIndices( );

    /// Creates the logical device and enables all device extensions provided if they are supported.
    /// @param queueFamilies All queue families whose queues should be created.
    /// @param extensions All device extensions that should be enabled.
    /// @return Returns true if the initilization was successful.
    UniqueDevice initDevice( std::vector<const char*>& extensions );

    /// Initializes the instance object and checks support for any given layer and extension.
    /// @param layers All validation layers that should be activated.
    /// @param extensions All instance extensions that should be activated.
    /// @note The given vector with the extensions will be appended by ones that are required by the window.
    /// @return Returns true if initialization was successful.
    UniqueInstance initInstance( const std::vector<const char*>& layers, std::vector<const char*>& extensions );

    bool initGraphicsPipelines( const std::vector<GraphicsPipelineCreateInfo>& createInfos );
    bool initRayTracingPipelines( const std::vector<RayTracingPipelineCreateInfoKHR> createInfos );
  } // namespace Initializer
} // namespace vk

#endif // INITIALIZERS_HPP