#include "api/utility/Helpers.hpp"

#include "api/Components.hpp"
#include "api/buffers/CommandBuffer.hpp"

namespace vk::Helper
{
  auto getPipelineShaderStageCreateInfo( ShaderStageFlagBits stage, ShaderModule module, const char* name, SpecializationInfo* specializationInfo ) -> PipelineShaderStageCreateInfo
  {
    PipelineShaderStageCreateInfo createInfo( { },                  // flags
                                              stage,                // stage
                                              module,               // module
                                              "main",               // pName
                                              specializationInfo ); // pSpecializationInfo

    return createInfo;
  }

  auto getImageCreateInfo( Extent3D extent ) -> ImageCreateInfo
  {
    ImageCreateInfo createInfo( { },                                                             // flags
                                ImageType::e2D,                                                  // imageType
                                Format::eR8G8B8A8Srgb,                                           // format
                                extent,                                                          // extent
                                1U,                                                              // mipLevels
                                1U,                                                              // arrayLayers
                                SampleCountFlagBits::e1,                                         // samples
                                ImageTiling::eOptimal,                                           // tiling
                                ImageUsageFlagBits::eTransferDst | ImageUsageFlagBits::eSampled, // usage
                                SharingMode::eExclusive,                                         // sharingMode
                                RAYEX_NAMESPACE::components::graphicsFamilyIndex,                // queueFamilyIndexCount
                                nullptr,                                                         // pQueueFamilyIndices
                                ImageLayout::eUndefined );                                       // initialLayout

    return createInfo;
  }

  auto getImageViewCreateInfo( Image image, Format format, ImageViewType viewType, ImageAspectFlags aspectFlags ) -> ImageViewCreateInfo
  {
    ComponentMapping components = { ComponentSwizzle::eIdentity,
                                    ComponentSwizzle::eIdentity,
                                    ComponentSwizzle::eIdentity,
                                    ComponentSwizzle::eIdentity };

    ImageSubresourceRange subresourceRange = { aspectFlags, // aspectMask
                                               0U,          // baseMipLevel
                                               1U,          // levelCount
                                               0U,          // baseArrayLayer
                                               1U };        // layerCount

    return ImageViewCreateInfo( { },                // flags
                                image,              // image
                                viewType,           // viewType
                                format,             // format
                                components,         // components
                                subresourceRange ); // subresourceRange
  }

  auto getSamplerCreateInfo( ) -> SamplerCreateInfo
  {
    SamplerCreateInfo createInfo( { },                          // flags
                                  Filter::eLinear,              // magFilter
                                  Filter::eLinear,              // minFilter
                                  SamplerMipmapMode::eLinear,   // mipmapMode
                                  SamplerAddressMode::eRepeat,  // addressModeU
                                  SamplerAddressMode::eRepeat,  // addressModeV
                                  SamplerAddressMode::eRepeat,  // addressModeW
                                  { },                          // mipLodBias,
                                  VK_TRUE,                      // anisotropyEnable
                                  16.0F,                        // maxAnisotropy
                                  VK_FALSE,                     // compareEnable
                                  CompareOp::eAlways,           // compareOp
                                  { },                          // minLod
                                  { },                          // maxLod
                                  BorderColor::eIntOpaqueBlack, // borderColor
                                  VK_FALSE );                   // unnormalizedCoordinates
    return createInfo;
  }

  auto getSubmitInfo( const Semaphore& waitSemaphore, const Semaphore& signalSemaphore, const std::vector<CommandBuffer>& commandBuffers, const PipelineStageFlags& pWaitDstStageMask ) -> SubmitInfo
  {
    return SubmitInfo( 1,                                               // waitSemaphoreCount
                       &waitSemaphore,                                  // pWaitSemaphores
                       &pWaitDstStageMask,                              // pWaitDstStageMask
                       static_cast<uint32_t>( commandBuffers.size( ) ), // commandBufferCount
                       commandBuffers.data( ),                          // pCommandBuffers
                       1,                                               // signalSemaphoreCount
                       &signalSemaphore );                              // pSignalSemaphores
  }

  auto getPresentInfoKHR( const Semaphore& waitSemaphore, uint32_t& imageIndex ) -> PresentInfoKHR
  {
    return PresentInfoKHR( 1,                                       // waitSemaphoreCount
                           &waitSemaphore,                          // pWaitSemaphores
                           1,                                       // swapchainCount
                           &RAYEX_NAMESPACE::components::swapchain, // pSwapchains
                           &imageIndex,                             // pImageIndices
                           nullptr );                               // pResults
  }

  auto findMemoryType( PhysicalDevice physicalDevice, uint32_t typeFilter, MemoryPropertyFlags properties ) -> uint32_t
  {
    static PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties( );

    for ( uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i )
    {
      if ( ( ( typeFilter & ( 1 << i ) ) != 0U ) && ( memoryProperties.memoryTypes[i].propertyFlags & properties ) == properties )
      {
        return i;
      }
    }

    RX_ERROR( "Failed to find suitable memory type" );
    return 0U;
  }

  auto unpack( const std::vector<UniqueImageView>& uniqueImageViews ) -> std::vector<ImageView>
  {
    std::vector<ImageView> result( uniqueImageViews.size( ) );

    for ( size_t i = 0; i < result.size( ); ++i )
    {
      result[i] = uniqueImageViews[i].get( );
    }

    return result;
  }

  auto unpack( const std::vector<UniqueFramebuffer>& uniqueFramebuffers ) -> std::vector<Framebuffer>
  {
    std::vector<Framebuffer> result( uniqueFramebuffers.size( ) );

    for ( size_t i = 0; i < result.size( ); ++i )
    {
      result[i] = uniqueFramebuffers[i].get( );
    }

    return result;
  }

  auto getPoolSizes( const std::vector<DescriptorSetLayoutBinding>& layoutBindings, uint32_t maxSets ) -> std::vector<DescriptorPoolSize>
  {
    std::vector<DescriptorPoolSize> result;
    result.reserve( layoutBindings.size( ) );

    for ( const auto& layoutBinding : layoutBindings )
    {
      DescriptorPoolSize poolSize( layoutBinding.descriptorType,    // type
                                   layoutBinding.descriptorCount ); // count

      result.push_back( poolSize );
    }

    return result;
  }

  void transitionImageLayout( Image image, ImageLayout oldLayout, ImageLayout newLayout )
  {
    auto barrierInfo = getImageMemoryBarrierInfo( image, oldLayout, newLayout );

    RAYEX_NAMESPACE::CommandBuffer commandBuffer;
    commandBuffer.init( RAYEX_NAMESPACE::components::graphicsCmdPool );
    commandBuffer.begin( );

    commandBuffer.get( 0 ).pipelineBarrier( std::get<1>( barrierInfo ), // srcStageMask
                                            std::get<2>( barrierInfo ), // dstStageMask
                                            DependencyFlagBits::eByRegion,
                                            0,
                                            nullptr,
                                            0,
                                            nullptr,
                                            1,
                                            &std::get<0>( barrierInfo ) ); // barrier

    commandBuffer.end( );
    commandBuffer.submitToQueue( RAYEX_NAMESPACE::components::graphicsQueue );
  }

  void transitionImageLayout( Image image, ImageLayout oldLayout, ImageLayout newLayout, CommandBuffer commandBuffer )
  {
    auto barrierInfo = getImageMemoryBarrierInfo( image, oldLayout, newLayout );

    commandBuffer.pipelineBarrier( std::get<1>( barrierInfo ), // srcStageMask
                                   std::get<2>( barrierInfo ), // dstStageMask
                                   DependencyFlagBits::eByRegion,
                                   0,
                                   nullptr,
                                   0,
                                   nullptr,
                                   1,
                                   &std::get<0>( barrierInfo ) ); // barrier
  }

  auto getImageMemoryBarrierInfo( Image image, ImageLayout oldLayout, ImageLayout newLayout, ImageSubresourceRange* subresourceRange ) -> std::tuple<ImageMemoryBarrier, PipelineStageFlags, PipelineStageFlags>
  {
    // TODO: not style conform.
    ImageMemoryBarrier barrier;
    barrier.oldLayout           = oldLayout;
    barrier.newLayout           = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image               = image;

    if ( subresourceRange == nullptr )
    {
      barrier.subresourceRange.aspectMask     = ImageAspectFlagBits::eColor;
      barrier.subresourceRange.baseMipLevel   = 0;
      barrier.subresourceRange.levelCount     = 1;
      barrier.subresourceRange.baseArrayLayer = 0;
      barrier.subresourceRange.layerCount     = 1;
    }
    else
    {
      barrier.subresourceRange = *subresourceRange;
    }

    PipelineStageFlags srcStageMask = PipelineStageFlagBits::eAllCommands;
    PipelineStageFlags dstStageMask = PipelineStageFlagBits::eAllCommands;

    if ( oldLayout == ImageLayout::eUndefined && newLayout == ImageLayout::eTransferDstOptimal )
    {
      barrier.srcAccessMask = { };
      barrier.dstAccessMask = AccessFlagBits::eTransferWrite;

      srcStageMask = PipelineStageFlagBits::eTopOfPipe;
      dstStageMask = PipelineStageFlagBits::eTransfer;
    }
    else if ( oldLayout == ImageLayout::eTransferDstOptimal && newLayout == ImageLayout::eShaderReadOnlyOptimal )
    {
      barrier.srcAccessMask = AccessFlagBits::eTransferWrite;
      barrier.dstAccessMask = AccessFlagBits::eShaderRead;

      srcStageMask = PipelineStageFlagBits::eTransfer;
      dstStageMask = PipelineStageFlagBits::eFragmentShader;
    }
    else if ( oldLayout == ImageLayout::eUndefined && newLayout == ImageLayout::eGeneral )
    {
      // nothing to do.
    }
    else if ( oldLayout == ImageLayout::eGeneral && newLayout == ImageLayout::eTransferSrcOptimal )
    {
      barrier.dstAccessMask = AccessFlagBits::eTransferRead;
    }
    else if ( oldLayout == ImageLayout::eTransferSrcOptimal && newLayout == ImageLayout::eGeneral )
    {
      barrier.srcAccessMask = AccessFlagBits::eTransferRead;
    }
    else if ( oldLayout == ImageLayout::eTransferDstOptimal && newLayout == ImageLayout::ePresentSrcKHR )
    {
      barrier.srcAccessMask = AccessFlagBits::eTransferWrite;
    }
    else if ( oldLayout == ImageLayout::eUndefined && newLayout == ImageLayout::ePresentSrcKHR )
    {
      barrier.srcAccessMask = { };
    }
    else if ( oldLayout == ImageLayout::ePresentSrcKHR && newLayout == ImageLayout::eColorAttachmentOptimal )
    {
      barrier.dstAccessMask = AccessFlagBits::eColorAttachmentWrite;
    }
    else if ( oldLayout == ImageLayout::eColorAttachmentOptimal && newLayout == ImageLayout::ePresentSrcKHR )
    {
      barrier.srcAccessMask = AccessFlagBits::eColorAttachmentWrite;
    }
    else if ( oldLayout == ImageLayout::eUndefined && newLayout == ImageLayout::eColorAttachmentOptimal )
    {
      barrier.srcAccessMask = { };
      barrier.dstAccessMask = AccessFlagBits::eColorAttachmentWrite;
    }
    else if ( oldLayout == ImageLayout::eTransferDstOptimal && newLayout == ImageLayout::eColorAttachmentOptimal )
    {
      barrier.srcAccessMask = AccessFlagBits::eTransferWrite;
      barrier.dstAccessMask = AccessFlagBits::eColorAttachmentWrite;
    }
    else
    {
      RX_ERROR( "Image layout transition not supported." );
    }

    return { barrier, srcStageMask, dstStageMask };
  }

  auto getAttachmentDescription( Format surfaceFormat ) -> AttachmentDescription
  {
    return AttachmentDescription( { },                           // flags
                                  surfaceFormat,                 // format
                                  SampleCountFlagBits::e1,       // samples
                                  AttachmentLoadOp::eClear,      // loadOp
                                  AttachmentStoreOp::eStore,     // storeOp
                                  AttachmentLoadOp::eDontCare,   // stencilLoadOp
                                  AttachmentStoreOp::eDontCare,  // stencilStoreOp
                                  ImageLayout::eUndefined,       // initialLayout
                                  ImageLayout::ePresentSrcKHR ); // finalLayout
  }

  auto getDepthAttachmentDescription( Format depthFormat ) -> AttachmentDescription
  {
    return AttachmentDescription( { },                                           // flags
                                  depthFormat,                                   // format
                                  SampleCountFlagBits::e1,                       // samples
                                  AttachmentLoadOp::eClear,                      // loadOp
                                  AttachmentStoreOp::eStore,                     // storeOp
                                  AttachmentLoadOp::eClear,                      // stencilLoadOp
                                  AttachmentStoreOp::eDontCare,                  // stencilStoreOp
                                  ImageLayout::eUndefined,                       // initialLayout
                                  ImageLayout::eDepthStencilAttachmentOptimal ); // finalLayout
  }

  auto isPhysicalDeviceQueueComplete( PhysicalDevice physicalDevice ) -> bool
  {
    auto queueFamilies           = physicalDevice.getQueueFamilyProperties( );
    auto queueFamilyIndicesCount = static_cast<uint32_t>( queueFamilies.size( ) );

    // Get all possible queue family indices with transfer support.
    std::vector<uint32_t> graphicsQueueFamilyIndices;
    std::vector<uint32_t> transferQueueFamilyIndices;
    std::vector<uint32_t> computeQueueFamilyIndices;

    for ( uint32_t index = 0; index < queueFamilyIndicesCount; ++index )
    {
      // Make sure the current queue family index contains at least one queue.
      if ( queueFamilies[index].queueCount == 0 )
      {
        continue;
      }

      if ( queueFamilies[index].queueFlags & QueueFlagBits::eGraphics )
      {
        if ( physicalDevice.getSurfaceSupportKHR( index, RAYEX_NAMESPACE::components::surface ) != 0U )
        {
          graphicsQueueFamilyIndices.push_back( index );
        }
      }

      if ( queueFamilies[index].queueFlags & QueueFlagBits::eTransfer )
      {
        transferQueueFamilyIndices.push_back( index );
      }

      if ( queueFamilies[index].queueFlags & QueueFlagBits::eCompute )
      {
        computeQueueFamilyIndices.push_back( index );
      }
    }

    if ( graphicsQueueFamilyIndices.empty( ) || computeQueueFamilyIndices.empty( ) || transferQueueFamilyIndices.empty( ) )
    {
      return false;
    }

    return true;
  }

  auto isPhysicalDeviceWithDedicatedTransferQueueFamily( PhysicalDevice physicalDevice ) -> bool
  {
    auto queueFamilyProperties = physicalDevice.getQueueFamilyProperties( );

    for ( auto& queueFamilyPropertie : queueFamilyProperties )
    {
      if ( !( queueFamilyPropertie.queueFlags & QueueFlagBits::eGraphics ) )
      {
        return true;
      }
    }

    return false;
  }

  auto evaluatePhysicalDevice( PhysicalDevice physicalDevice ) -> std::pair<unsigned int, std::string>
  {
    unsigned int score = 0U;

    // Check the device's features and properties.
    auto props = physicalDevice.getProperties( );
    auto feats = physicalDevice.getFeatures( );

    PhysicalDeviceRayTracingFeaturesKHR rayTracingFeatures;

    PhysicalDeviceDescriptorIndexingFeatures indexingFeatures;
    rayTracingFeatures.pNext = &indexingFeatures;

    PhysicalDeviceBufferDeviceAddressFeatures deviceAddressFeatures;
    indexingFeatures.pNext = &deviceAddressFeatures;

    PhysicalDeviceRobustness2FeaturesEXT robustness2Features;
    deviceAddressFeatures.pNext = &robustness2Features;

    PhysicalDeviceFeatures2 features2;
    features2.pNext = &rayTracingFeatures;

    physicalDevice.getFeatures2( &features2 );

    std::string deviceName = props.deviceName;

    if ( indexingFeatures.runtimeDescriptorArray == VK_FALSE ||
         indexingFeatures.shaderStorageBufferArrayNonUniformIndexing == VK_FALSE ||
         indexingFeatures.descriptorBindingVariableDescriptorCount == VK_FALSE ||
         indexingFeatures.descriptorBindingPartiallyBound == VK_FALSE ||
         indexingFeatures.descriptorBindingStorageBufferUpdateAfterBind == VK_FALSE ||
         indexingFeatures.descriptorBindingUpdateUnusedWhilePending == VK_FALSE ||
         indexingFeatures.descriptorBindingSampledImageUpdateAfterBind == VK_FALSE ||
         indexingFeatures.shaderSampledImageArrayNonUniformIndexing == VK_FALSE ||
         rayTracingFeatures.rayTracing == VK_FALSE ||
         rayTracingFeatures.rayQuery == VK_FALSE ||
         feats.samplerAnisotropy == VK_FALSE ||
         feats.shaderInt64 == VK_FALSE ||
         deviceAddressFeatures.bufferDeviceAddress == VK_FALSE ||
         robustness2Features.nullDescriptor == VK_FALSE )
    {
      return { 0U, deviceName };
    }

    if ( props.deviceType == PhysicalDeviceType::eDiscreteGpu )
    {
      score += 100U;
    }
    else
    {
      return { 0U, deviceName };
    }

#ifdef VK_API_VERSION_1_2
    if ( props.apiVersion >= VK_API_VERSION_1_2 )
    {
      score += 10U;
    }
    else
    {
      return { 0U, deviceName };
    }
#endif

    if ( deviceName.find( "RTX" ) != std::string::npos )
    {
      score += 100U;
    }

    if ( isPhysicalDeviceQueueComplete( physicalDevice ) )
    {
      score += 100U;
    }
    else
    {
      return { 0U, deviceName };
    }

    if ( isPhysicalDeviceWithDedicatedTransferQueueFamily( physicalDevice ) )
    {
      score += 25;
    }

    // TODO: add more hardware specific evaulation (those that are benefitial for path tracing)
    // https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#features

    return { score, deviceName };
  }

  void checkDeviceExtensionSupport( const std::vector<const char*>& extensions )
  {
    // Stores the name of the extension and a bool that tells if they were found or not.
    std::map<const char*, bool> requiredExtensions;

    for ( const auto& extension : extensions )
    {
      requiredExtensions.emplace( extension, false );
    }

    std::vector<ExtensionProperties> physicalDeviceExtensions = RAYEX_NAMESPACE::components::physicalDevice.enumerateDeviceExtensionProperties( );

    // Iterates over all enumerated physical device extensions to see if they are available.
    for ( const auto& physicalDeviceExtension : physicalDeviceExtensions )
    {
      for ( auto& requiredphysicalDeviceExtension : requiredExtensions )
      {
        if ( strcmp( physicalDeviceExtension.extensionName, requiredphysicalDeviceExtension.first ) == 0 )
        {
          requiredphysicalDeviceExtension.second = true;
        }
      }
    }

    // Give feedback on the previous operations.
    for ( const auto& requiredphysicalDeviceExtension : requiredExtensions )
    {
      if ( !requiredphysicalDeviceExtension.second )
      {
        RX_FATAL( "Missing physical device extension: ", requiredphysicalDeviceExtension.first, ". Perhaps you have not installed the NVIDIA Vulkan Beta drivers?" );
      }
      else
      {
        RX_VERBOSE( "Added device extension: ", requiredphysicalDeviceExtension.first );
      }
    }
  }

  void checkInstanceLayersSupport( const std::vector<const char*>& layers )
  {
    auto properties = vk::enumerateInstanceLayerProperties( );

    for ( const char* name : layers )
    {
      bool found = false;
      for ( const auto& property : properties )
      {
        if ( strcmp( property.layerName, name ) == 0 )
        {
          found = true;
          break;
        }
      }

      if ( !found )
      {
        RX_FATAL( "Validation layer ", name, " is not available on this device." );
      }

      RX_VERBOSE( "Added layer: ", name, "." );
    }
  }

  void checkInstanceExtensionsSupport( const std::vector<const char*>& extensions )
  {
    auto properties = vk::enumerateInstanceExtensionProperties( );

    for ( const char* name : extensions )
    {
      bool found = false;
      for ( const auto& property : properties )
      {
        if ( strcmp( property.extensionName, name ) == 0 )
        {
          found = true;
          break;
        }
      }

      if ( !found )
      {
        RX_FATAL( "Instance extensions ", name, " is not available on this device." );
      }

      RX_VERBOSE( "Added instance extension: ", name, "." );
    }
  }

  auto findSupportedImageFormat( vk::PhysicalDevice physicalDevice, const std::vector<vk::Format>& formatsToTest, vk::FormatFeatureFlagBits features, vk::ImageTiling tiling ) -> vk::Format
  {
    for ( vk::Format format : formatsToTest )
    {
      auto props = physicalDevice.getFormatProperties( format );

      if ( tiling == vk::ImageTiling::eLinear && ( props.linearTilingFeatures & features ) == features )
      {
        return format;
      }
      if ( tiling == vk::ImageTiling::eOptimal && ( props.optimalTilingFeatures & features ) == features )
      {
        return format;
      }
    }

    RX_ERROR( "Failed to retrieve any supported image format." );

    return vk::Format::eUndefined;
  }
} // namespace vk::Helper
