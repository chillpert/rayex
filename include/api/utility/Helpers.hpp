#ifndef HELPERS_HPP
#define HELPERS_HPP

#include "api/raytracing/AccelerationStructure.hpp"
#include "base/Model.hpp"
#include "api/RenderPass.hpp"

namespace vk
{
  /// A collection of helper functions for various purposes such as simplifying the process of filling Vulkan structs and objects with their respective parameters.
  /// @ingroup API
  namespace Helper
  {
    /// Simplifies the process of setting up a pipeline shader stage create info.
    /// @param stage The target shader stages.
    /// @param module The vulkan shader module.
    /// @param name The entry function inside the shader file.
    /// @param specializationInfo For specifying constant values at the time the vulkan pipeline is created.
    /// @return Returns the completed pipeline shader stage create info.
    PipelineShaderStageCreateInfo getPipelineShaderStageCreateInfo( ShaderStageFlagBits stage, ShaderModule module, const char* name = "main", SpecializationInfo* specializationInfo = nullptr );

    /// Simplifies the process of setting up an image create info with the most common settings.
    /// @param extent The image's extent.
    /// @return Returns the completed image create info.
    ImageCreateInfo getImageCreateInfo( Extent3D extent );

    /// Simplifies the process of setting up a sampler create info for texture usage.
    /// @return Returns the completed sampler create info.
    SamplerCreateInfo getSamplerCreateInfo( );

    /// Simplifies the process of setting up a submit info for the rendering process.
    /// @param waitSemaphore The wait semaphore. Only one wait semaphore can be passed.
    /// @param signalSemaphore The signal semaphore. Only one signal semaphore can be passed.
    /// @param commandBuffers A vector of vulkan command buffers which contain the recorded draw calls.
    /// @param pWaitDstStageMask Specifies a pipeline stage at which the semaphore wait will occur.
    /// @return Returns the completed submit info.
    SubmitInfo getSubmitInfo( const Semaphore& waitSemaphore, const Semaphore& signalSemaphore, const std::vector<CommandBuffer>& commandBuffers, const PipelineStageFlags& pWaitDstStageMask );

    /// Simplifies the process of setting up a present info for the rendering process.
    /// @param waitSemaphore The wait semaphore. Only one wait semaphore can be passed.
    /// @param imageIndex The current image index that will be used to access the correct image in the swapchain.
    /// @return Returns the completed present info.
    PresentInfoKHR getPresentInfoKHR( const Semaphore& waitSemaphore, uint32_t& imageIndex );

    /// Helps finding the suitable memory type based on the function input.
    /// @param physicalDevice The GPU that you want to find the correct memory type for.
    /// @param typeFiler Is a bitmask containing one bit set for every memory type which the specified windows handle can be imported as.
    /// @param properties The memory property flags.
    /// @return Returns the memory type matching the function input.
    uint32_t findMemoryType( PhysicalDevice physicalDevice, uint32_t typeFilter, MemoryPropertyFlags properties );

    /// A template function for unwrapping data structures with a unique handle.
    /// @param data A vector of any data structure with a unique handle.
    /// @return Returns a vector of the given data type but without the unique handles.
    template <typename Out, typename In>
    std::vector<Out> unpack( const std::vector<In>& data )
    {
      std::vector<Out> result( data.size( ) );

      for ( size_t i = 0; i < result.size( ); ++i )
        result[i] = data[i].get( );

      return result;
    }

    /// Converts a map of rx models to a vector of rx models.
    /// @param models A map of rx models with their model path as key.
    /// @return Returns a vector of the given rx models.
    std::vector<std::shared_ptr<RENDERER_NAMESPACE::Model>> unpack( const std::unordered_map<std::string, std::shared_ptr<RENDERER_NAMESPACE::Model>>& models );

    /// Returns the descriptor pool sizes required by any given descriptor set layout bindings.
    /// @param layoutBinding A vector of descriptor set layout bindings that will be used to create the descriptor pool sizes.
    /// @return Returns a vector of descriptor pool sizes.
    std::vector<DescriptorPoolSize> getPoolSizes( const std::vector<DescriptorSetLayoutBinding>& layoutBindings );

    /// Transitions the image layout of any given image. The function will generate its own command buffer.
    /// @param image The vulkan image for which you want to change the image layout.
    /// @param oldLayout The current image layout of the given vulkan image.
    /// @param newLayout The target image layout.
    void transitionImageLayout( Image image, ImageLayout oldLayout, ImageLayout newLayout );
    
    /// Transitions the image layout of any given image using an already existing command buffer. 
    /// @param image The vulkan image for which you want to change the image layout.
    /// @param oldLayout The current image layout of the given vulkan image.
    /// @param newLayout The target image layout.
    /// @param commandBuffer The command buffer that will be used. It must be in the recording stage.
    void transitionImageLayout( Image image, ImageLayout oldLayout, ImageLayout newLayout, CommandBuffer commandBuffer );

    /// Simplifies the process of setting up an image memory barrier info.
    /// @param image The vulkan image.
    /// @param oldLayout The current image layout of the given vulkan image.
    /// @param newLayout The target image layout.
    /// @return Returns a tuple containing the actual image memory barrier as well as the source stage mask and the destination stage mask. 
    std::tuple<ImageMemoryBarrier, PipelineStageFlags, PipelineStageFlags> getImageMemoryBarrierInfo( Image image, ImageLayout oldLayout, ImageLayout newLayout );

    /// Simplifies the process of setting up an attachment description for a render pass.
    /// @param surfaceFormat The surface's format.
    /// @return Returns the completed attachment description.
    AttachmentDescription getAttachmentDescription( Format surfaceFormat );

    /// Simplifies the process of setting up a depth attachment description for a render pass.
    /// @param depthFormat The depth format.
    /// @return Returns the completed depth attachment description.
    AttachmentDescription getDepthAttachmentDescription( Format depthFormat );

    /// Simplifies the process of setting up a descriptor set layout binding.
    /// @binding The binding in the shader.
    /// @descriptorType The type of the descriptor.
    /// @stageFlags The type of shader in which the descriptor can be used.
    /// @return Returns the descriptor set layout binding.
    DescriptorSetLayoutBinding getDescriptorSetLayoutBinding( uint32_t binding, DescriptorType descriptorType, ShaderStageFlags stageFlags );

    /// Checks if a given physical device supports the queue capabilities required by the application.
    /// @param physicalDevice The physical device to check.
    bool isPhysicalDeviceQueueComplete( PhysicalDevice physicalDevice );

    /// Checks if a given physical device has a dedicated transfer queue family index that is not a graphics queue family index.
    /// @param physicalDevice The physical device to check.
    /// @return Returns true if the physical device has a dedicated transfer queue family index.
    bool isPhysicalDeviceWithDedicatedTransferQueueFamily( PhysicalDevice physicalDevice );

    /// Scores a given physical device (GPU).
    /// @param physicalDevice The physical device to score.
    /// @return Returns a pair consisting out of the determined score and the name of the physical device.
    /// @note RTX hardware surpasses any other GPU.
    std::pair<unsigned int, std::string> evaluatePhysicalDevice( PhysicalDevice physicalDevice );

    /// Checks if all device extensions provided are supported.
    /// 
    /// If any device extension is not supported the application will shut down immediately.
    /// @param extensions All device extensions that should be activated.
    bool checkDeviceExtensionSupport( const std::vector<const char*>& extensions );

    /// Checks if all layers provided are supported.
    /// 
    /// If any layer is not supported the application will shut down immediately.
    /// @param layers All validation layers that should be activated.
    /// @return Returns true if initialization was successful.
    bool checkInstanceLayersSupport( Instance instance, const std::vector<const char*>& layers );

    /// Checks if all instance extensions provided are supported.
    /// 
    /// If any instance extension is not supported the application will shut down immediately.
    /// @param extensions All instance extensions that should be activated.
    /// @return Returns true if initialization was successful.
    bool checkInstanceExtensionsSupport( Instance instance, const std::vector<const char*>& extensions );
  }
} 

#endif // HELPERS_HPP