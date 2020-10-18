#ifndef HELPERS_HPP
#define HELPERS_HPP

#include "api/RenderPass.hpp"
#include "api/raytracing/AccelerationStructure.hpp"

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
    auto getPipelineShaderStageCreateInfo( ShaderStageFlagBits stage, ShaderModule module, const char* name = "main", SpecializationInfo* specializationInfo = nullptr ) -> PipelineShaderStageCreateInfo;

    /// Simplifies the process of setting up an image create info with the most common settings.
    /// @param extent The image's extent.
    /// @return Returns the completed image create info.
    auto getImageCreateInfo( Extent3D extent ) -> ImageCreateInfo;

    /// Simplifies the process of setting up a sampler create info for texture usage.
    /// @return Returns the completed sampler create info.
    auto getSamplerCreateInfo( ) -> SamplerCreateInfo;

    /// Simplifies the process of setting up a submit info for the rendering process.
    /// @param waitSemaphore The wait semaphore. Only one wait semaphore can be passed.
    /// @param signalSemaphore The signal semaphore. Only one signal semaphore can be passed.
    /// @param commandBuffers A vector of vulkan command buffers which contain the recorded draw calls.
    /// @param pWaitDstStageMask Specifies a pipeline stage at which the semaphore wait will occur.
    /// @return Returns the completed submit info.
    auto getSubmitInfo( const Semaphore& waitSemaphore, const Semaphore& signalSemaphore, const std::vector<CommandBuffer>& commandBuffers, const PipelineStageFlags& pWaitDstStageMask ) -> SubmitInfo;

    /// Simplifies the process of setting up a present info for the rendering process.
    /// @param waitSemaphore The wait semaphore. Only one wait semaphore can be passed.
    /// @param imageIndex The current image index that will be used to access the correct image in the swapchain.
    /// @return Returns the completed present info.
    auto getPresentInfoKHR( const Semaphore& waitSemaphore, uint32_t& imageIndex ) -> PresentInfoKHR;

    /// Helps finding the suitable memory type based on the function input.
    /// @param physicalDevice The GPU that you want to find the correct memory type for.
    /// @param typeFilter Is a bitmask containing one bit set for every memory type which the specified windows handle can be imported as.
    /// @param properties The memory property flags.
    /// @return Returns the memory type matching the function input.
    auto findMemoryType( PhysicalDevice physicalDevice, uint32_t typeFilter, MemoryPropertyFlags properties ) -> uint32_t;

    /// A template function for unwrapping data structures with a unique handle.
    /// @param data A vector of any data structure with a unique handle.
    /// @return Returns a vector of the given data type but without the unique handles.
    template <typename Out, typename In>
    auto unpack( const std::vector<In>& data ) -> std::vector<Out>
    {
      std::vector<Out> result( data.size( ) );

      for ( size_t i = 0; i < result.size( ); ++i )
        result[i] = data[i].get( );

      return result;
    }

    /// Returns the descriptor pool sizes required by any given descriptor set layout bindings.
    /// @param layoutBindings A vector of descriptor set layout bindings that will be used to create the descriptor pool sizes.
    /// @param maxSets The maximum number of descriptor sets.
    /// @return Returns a vector of descriptor pool sizes.
    auto getPoolSizes( const std::vector<DescriptorSetLayoutBinding>& layoutBindings, uint32_t maxSets ) -> std::vector<DescriptorPoolSize>;

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
    auto getImageMemoryBarrierInfo( Image image, ImageLayout oldLayout, ImageLayout newLayout ) -> std::tuple<ImageMemoryBarrier, PipelineStageFlags, PipelineStageFlags>;

    /// Simplifies the process of setting up an attachment description for a render pass.
    /// @param surfaceFormat The surface's format.
    /// @return Returns the completed attachment description.
    auto getAttachmentDescription( Format surfaceFormat ) -> AttachmentDescription;

    /// Simplifies the process of setting up a depth attachment description for a render pass.
    /// @param depthFormat The depth format.
    /// @return Returns the completed depth attachment description.
    auto getDepthAttachmentDescription( Format depthFormat ) -> AttachmentDescription;

    /// Checks if a given physical device supports the queue capabilities required by the application.
    /// @param physicalDevice The physical device to check.
    auto isPhysicalDeviceQueueComplete( PhysicalDevice physicalDevice ) -> bool;

    /// Checks if a given physical device has a dedicated transfer queue family index that is not a graphics queue family index.
    /// @param physicalDevice The physical device to check.
    /// @return Returns true if the physical device has a dedicated transfer queue family index.
    auto isPhysicalDeviceWithDedicatedTransferQueueFamily( PhysicalDevice physicalDevice ) -> bool;

    /// Scores a given physical device (GPU).
    /// @param physicalDevice The physical device to score.
    /// @return Returns a pair consisting out of the determined score and the name of the physical device.
    /// @note RTX hardware surpasses any other GPU.
    auto evaluatePhysicalDevice( PhysicalDevice physicalDevice ) -> std::pair<unsigned int, std::string>;

    /// Checks if all device extensions provided are supported.
    ///
    /// If any device extension is not supported the application will shut down immediately.
    /// @param extensions All device extensions that should be activated.
    void checkDeviceExtensionSupport( const std::vector<const char*>& extensions );

    /// Checks if all layers provided are supported.
    ///
    /// If any layer is not supported the application will shut down immediately.
    /// @param layers All validation layers that should be activated.
    /// @return Returns true if initialization was successful.
    void checkInstanceLayersSupport( const std::vector<const char*>& layers );

    /// Checks if all instance extensions provided are supported.
    ///
    /// If any instance extension is not supported the application will shut down immediately.
    /// @param extensions All instance extensions that should be activated.
    /// @return Returns true if initialization was successful.
    void checkInstanceExtensionsSupport( const std::vector<const char*>& extensions );
  } // namespace Helper
} // namespace vk

#endif // HELPERS_HPP
