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
    CommandPool initCommandPool(uint32_t queueFamilyIndex, CommandPoolCreateFlags flags = { } );
    
    /// Creates a descriptor pool with a unique handle.
    /// @param poolSizes All descriptor types for descriptor sets that can be allocated from this descriptor pool.
    /// @param maxSets The maximum amount of descriptor sets that can be allocated from this descriptor pool.
    /// @param flags The flags used for creating the descriptor pool.
    /// @return Returns the descriptor pool.  
    UniqueDescriptorPool initDescriptorPoolUnique( const std::vector<DescriptorPoolSize>& poolSizes, uint32_t maxSets = 1, DescriptorPoolCreateFlags flags = { } );

    /// Creates a descriptor pool.
    /// @param poolSizes All descriptor types for descriptor sets that can be allocated from this descriptor pool.
    /// @param maxSets The maximum amount of descriptor sets that can be allocated from this descriptor pool.
    /// @param flags The flags used for creating the descriptor pool.
    /// @return Returns the descriptor pool.  
    DescriptorPool initDescriptorPool( const std::vector<DescriptorPoolSize>& poolSizes, uint32_t maxSets, DescriptorPoolCreateFlags flags = { } );

    /// Allocates and binds unique memory for an image.
    /// @param image The image to allocate memory for.
    /// @param pNext The pNext chain of the Vulkan allocation info.
    /// @return Returns the memory.
    UniqueDeviceMemory allocateMemoryUnique( Image image, MemoryPropertyFlags propertyFlags = MemoryPropertyFlagBits::eDeviceLocal, void * pNext = nullptr );

    /// Allocates and binds memory for an image.
    /// @param image The image to allocate memory for.
    /// @param pNext The pNext chain of the Vulkan allocation info.
    /// @return Returns the memory.
    DeviceMemory allocateMemory( Image image, MemoryPropertyFlags propertyFlags = MemoryPropertyFlagBits::eDeviceLocal, void * pNext = nullptr );

    /// Allocates and binds unique memory for a buffer.
    /// @param image The image to allocate memory for.
    /// @param pNext The pNext chain of the Vulkan allocation info.
    /// @return Returns the memory.
    UniqueDeviceMemory allocateMemoryUnique( Buffer buffer, MemoryPropertyFlags propertyFlags = MemoryPropertyFlagBits::eDeviceLocal, void * pNext = nullptr );

    /// Allocates and binds unique memory for an acceleration structure object.
    /// @param as The rx::AccelerationStructure object the memory will be allocated for.
    void allocateMemory( rx::AccelerationStructure& as );

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
    /// Note: Use rx::Helper::getSamplerCreateInfo to get a predefined init info.
    UniqueSampler initSamplerUnique( const SamplerCreateInfo& initInfo );

    /// Create a sampler.
    /// @param initInfo The Vulkan init info for the sampler.
    /// @return Returns the sampler.
    /// Note: Use rx::Helper::getSamplerCreateInfo to get a predefined init info.
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
    UniqueShaderModule initShaderModuleUnique( const std::string& path );

    /// Parses a given shader and inits a shader module.
    /// @param path The full path to the GLSL shader file.
    /// @return Returns the shader module.
    ShaderModule initShaderModule( const std::string& path );

    /// Creates the acceleration structure and allocates and binds memory for it.
    /// @param asCreateInfo The Vulkan init info for the acceleration structure.
    /// @return Returns an rx::AccelerationStructure object that contains the AS itself as well as the memory for it.
    rx::AccelerationStructure initAccelerationStructure( const AccelerationStructureCreateInfoKHR& asCreateInfo );

    /// Retrieves the most suited GPU on the current machine.
    ///
    /// If a device is found its properties and features will be retrieved.
    /// @note Multiple GPUs can not be utilized.
    std::pair<PhysicalDevice, bool> initPhysicalDevice( );
  }
}

#endif // INITIALIZERS_HPP