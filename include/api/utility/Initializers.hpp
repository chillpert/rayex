#ifndef INITIALIZERS_HPP
#define INITIALIZERS_HPP

#include "api/Components.hpp"
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
    auto initFenceUnique( FenceCreateFlags flags = FenceCreateFlagBits::eSignaled ) -> UniqueFence;

    /// Creates a fence.
    /// @param flags The flags used for creating the fence.
    /// @return Returns the fence.
    auto initFence( FenceCreateFlags flags = FenceCreateFlagBits::eSignaled ) -> Fence;

    /// Creates a semaphore with a unique handle.
    /// @param flags The flags used for creating the semaphore.
    /// @return Returns the semaphore.
    auto initSemaphoreUnique( SemaphoreCreateFlags flags = { } ) -> UniqueSemaphore;

    /// Creates a semaphore.
    /// @param flags The flags used for creating the semaphore.
    /// @return Returns the semaphore.
    auto initSemaphore( SemaphoreCreateFlags flags = { } ) -> Semaphore;

    /// Creates a command pool with a unique handle.
    /// @param queueFamilyIndex The queue family from which the command pool can submit to.
    /// @param flags The flags used for creating the command pool.
    /// @return Returns the command pool.
    auto initCommandPoolUnique( uint32_t queueFamilyIndex, CommandPoolCreateFlags flags = { } ) -> UniqueCommandPool;

    /// Creates a command pool.
    /// @param queueFamilyIndex The queue family from which the command pool can submit to.
    /// @param flags The flags used for creating the command pool.
    /// @return Returns the command pool.
    auto initCommandPool( uint32_t queueFamilyIndex, CommandPoolCreateFlags flags = { } ) -> CommandPool;

    /// Creates a unique descriptor pool.
    /// @param poolSizes A vector of Vulkan descriptor pool sizes.
    /// @param maxSets The maximum amount of descriptor sets that can be allocated from this pool.
    /// @param flags Flags for the creation process.
    auto initDescriptorPoolUnique( const std::vector<DescriptorPoolSize>& poolSizes, uint32_t maxSets = 1, DescriptorPoolCreateFlags flags = { } ) -> UniqueDescriptorPool;

    /// Creates unique Vulkan descriptor sets.
    /// @param pool A unique Vulkan descriptor pool to allocate the sets from.
    /// @param layout The desired unique Vulkan descriptor set layout.
    auto initDescriptorSetsUnique( const UniqueDescriptorPool& pool, const UniqueDescriptorSetLayout& layout ) -> std::vector<DescriptorSet>;

    /// Allocates and binds unique memory for an image.
    /// @param image The image to allocate memory for.
    /// @param pNext The pNext chain of the Vulkan allocation info.
    /// @return Returns the memory.
    auto allocateMemoryUnique( Image image, MemoryPropertyFlags propertyFlags = MemoryPropertyFlagBits::eDeviceLocal, void* pNext = nullptr ) -> UniqueDeviceMemory;

    /// Allocates and binds memory for an image.
    /// @param image The image to allocate memory for.
    /// @param pNext The pNext chain of the Vulkan allocation info.
    /// @return Returns the memory.
    auto allocateMemory( Image image, MemoryPropertyFlags propertyFlags = MemoryPropertyFlagBits::eDeviceLocal, void* pNext = nullptr ) -> DeviceMemory;

    /// Allocates and binds unique memory for a buffer.
    /// @param image The image to allocate memory for.
    /// @param pNext The pNext chain of the Vulkan allocation info.
    /// @return Returns the memory.
    auto allocateMemoryUnique( Buffer buffer, MemoryPropertyFlags propertyFlags = MemoryPropertyFlagBits::eDeviceLocal, void* pNext = nullptr ) -> UniqueDeviceMemory;

    /// Allocates and binds unique memory for an acceleration structure object.
    /// @param as The RAYEXEC_NAMESPACE::AccelerationStructure object the memory will be allocated for.
    void allocateMemory( RAYEXEC_NAMESPACE::AccelerationStructure& as );

    /// Allocates and binds memory for a buffer.
    /// @param image The image to allocate memory for.
    /// @param pNext The pNext chain of the Vulkan allocation info.
    /// @return Returns the memory.
    auto allocateMemory( Buffer buffer, MemoryPropertyFlags propertyFlags = MemoryPropertyFlagBits::eDeviceLocal, void* pNext = nullptr ) -> DeviceMemory;

    /// Create an image view with a unique handle.
    /// @param image The image to init an image view for.
    /// @param format The target format of the image view.
    /// @return Returns the image view.
    auto initImageViewUnique( Image image, Format format, ImageAspectFlags aspectFlags = ImageAspectFlagBits::eColor ) -> UniqueImageView;

    /// Create an image view.
    /// @param image The image to init an image view for.
    /// @param format The target format of the image view.
    /// @return Returns the image view.
    auto initImageView( Image image, Format format, ImageAspectFlags aspectFlags = ImageAspectFlagBits::eColor ) -> ImageView;

    /// Create a sampler with a unique handle.
    /// @param initInfo The Vulkan init info for the sampler.
    /// @return Returns the sampler.
    /// Note: Use RAYEXEC_NAMESPACE::Helper::getSamplerCreateInfo to get a predefined init info.
    auto initSamplerUnique( const SamplerCreateInfo& createInfo ) -> UniqueSampler;

    /// Create a sampler.
    /// @param initInfo The Vulkan init info for the sampler.
    /// @return Returns the sampler.
    /// Note: Use RAYEXEC_NAMESPACE::Helper::getSamplerCreateInfo to get a predefined init info.
    auto initSampler( const SamplerCreateInfo& createInfo ) -> Sampler;

    /// Create a framebuffer with a unique handle.
    /// @param attachments All the image view attachments for the framebuffer.
    /// @param renderPass The render pass for which the framebuffer will be used.
    /// @param extent The extent of the framebuffer.
    /// @return Returns the framebuffer.
    auto initFramebufferUnique( const std::vector<ImageView>& attachments, RenderPass renderPass, const Extent2D& extent ) -> UniqueFramebuffer;

    /// Create a framebuffer.
    /// @param attachments All the image view attachments for the framebuffer.
    /// @param renderPass The render pass for which the framebuffer will be used.
    /// @param extent The extent of the framebuffer.
    /// @return Returns the framebuffer.
    auto initFramebuffer( const std::vector<ImageView>& attachments, RenderPass renderPass, const Extent2D& extent ) -> Framebuffer;

    /// Create a query pool with a unique handle.
    /// @param count The number of queries managed by the pool.
    /// @param type Specifies the type of queries managed by the pool.
    /// return Returns the query pool.
    auto initQueryPoolUnique( uint32_t count, QueryType type ) -> UniqueQueryPool;

    /// Create a query pool.
    /// @param count The number of queries managed by the pool.
    /// @param type Specifies the type of queries managed by the pool.
    /// return Returns the query pool.
    auto initQueryPool( uint32_t count, QueryType type ) -> QueryPool;

    /// Parses a given shader and inits a shader module with a unique handle.
    /// @param path The full path to the GLSL shader file.
    /// @return Returns the shader module.
    auto initShaderModuleUnique( std::string_view path ) -> UniqueShaderModule;

    /// Parses a given shader and inits a shader module.
    /// @param path The full path to the GLSL shader file.
    /// @return Returns the shader module.
    auto initShaderModule( std::string_view path ) -> ShaderModule;

    /// Creates the acceleration structure and allocates and binds memory for it.
    /// @param asCreateInfo The Vulkan init info for the acceleration structure.
    /// @return Returns an RAYEXEC_NAMESPACE::AccelerationStructure object that contains the AS itself as well as the memory for it.
    auto initAccelerationStructure( const AccelerationStructureCreateInfoKHR& asCreateInfo ) -> RAYEXEC_NAMESPACE::AccelerationStructure;

    /// Retrieves the most suited GPU on the current machine.
    ///
    /// If a device is found its properties and features will be retrieved.
    /// @return Returns true if the initilization was successful.
    /// @note Multiple GPUs can not be utilized.
    auto initPhysicalDevice( ) -> PhysicalDevice;

    /// Retrieves the queue family indices for both graphics and transfer queue families.
    /// @note The function tries to find a transfer queue family index that is not the same as the graphics queue family index.
    /// @see hasDedicatedTransferQueueFamily(vk::PhysicalDevice)
    void initQueueFamilyIndices( );

    /// Creates the logical device and enables all device extensions provided if they are supported.
    /// @param queueFamilies All queue families whose queues should be created.
    /// @param extensions All device extensions that should be enabled.
    /// @return Returns true if the initilization was successful.
    auto initDevice( std::vector<const char*>& extensions ) -> UniqueDevice;

    /// Initializes the instance object and checks support for any given layer and extension.
    /// @param layers All validation layers that should be activated.
    /// @param extensions All instance extensions that should be activated.
    /// @note The given vector with the extensions will be appended by ones that are required by the window.
    /// @return Returns true if initialization was successful.
    auto initInstance( const std::vector<const char*>& layers, std::vector<const char*>& extensions ) -> UniqueInstance;

    auto initGraphicsPipelinesUnique( const std::vector<GraphicsPipelineCreateInfo>& createInfos ) -> std::vector<UniquePipeline>;

    auto initRayTracingPipelinesUnique( const std::vector<RayTracingPipelineCreateInfoKHR>& createInfos ) -> std::vector<UniquePipeline>;

  } // namespace Initializer
} // namespace vk

#endif // INITIALIZERS_HPP
