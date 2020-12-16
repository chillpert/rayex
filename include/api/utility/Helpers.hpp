#pragma once

#include "api/RenderPass.hpp"
#include "api/pathtrace/AccelerationStructure.hpp"

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

    /// Simplifies the process of setting up an image view create info with the most common settings.
    /// @param image The image to init an image view for.
    /// @param format The target format of the image view.
    /// @param viewType The image's view type.
    /// @param aspectFlags The image's aspect flags.
    /// @return Returns the image view create info.
    auto getImageViewCreateInfo( Image image, Format format, ImageViewType viewType = ImageViewType::e2D, ImageAspectFlags aspectFlags = ImageAspectFlagBits::eColor ) -> ImageViewCreateInfo;

    /// Simplifies the process of setting up a sampler create info for texture usage.
    /// @return Returns the completed sampler create info.
    auto getSamplerCreateInfo( ) -> SamplerCreateInfo;

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
    /// @param subresourceRange The image view's subresource range.
    /// @return Returns a tuple containing the actual image memory barrier as well as the source stage mask and the destination stage mask.
    auto getImageMemoryBarrierInfo( Image image, ImageLayout oldLayout, ImageLayout newLayout, const ImageSubresourceRange* subresourceRange = nullptr ) -> std::tuple<ImageMemoryBarrier, PipelineStageFlags, PipelineStageFlags>;

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

    /// Retrieves a supported image format for a given physical device.
    /// @param physicalDevice The physical device to check.
    /// @param formatsToTest A vector of formats that will be checked for their support.
    /// @param features The Vulkan format features.
    /// @param tiling The Vulkan image tiling.
    auto findSupportedImageFormat( vk::PhysicalDevice physicalDevice, const std::vector<vk::Format>& formatsToTest, vk::FormatFeatureFlagBits features, vk::ImageTiling tiling ) -> vk::Format;
  } // namespace Helper
} // namespace vk
