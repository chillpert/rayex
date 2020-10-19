#include "api/utility/Initializers.hpp"

#include "api/Components.hpp"
#include "api/Vertex.hpp"
#include "api/buffers/CommandBuffer.hpp"
#include "api/utility/Helpers.hpp"
#include "api/utility/Util.hpp"

#define VULKAN_HPP_STORAGE_SHARED
#define VULKAN_HPP_STORAGE_SHARED_EXPORT
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace vk::Initializer
{
  auto initFenceUnique( FenceCreateFlags flags ) -> UniqueFence
  {
    FenceCreateInfo createInfo( flags );

    UniqueFence fence = RAYEX_NAMESPACE::g_device.createFenceUnique( createInfo );
    RX_ASSERT( fence, "Failed to create fence." );

    return fence;
  }

  auto initFence( FenceCreateFlags flags ) -> Fence
  {
    FenceCreateInfo createInfo( flags );

    Fence fence = RAYEX_NAMESPACE::g_device.createFence( createInfo );
    RX_ASSERT( fence, "Failed to create fence." );

    return fence;
  }

  auto initSemaphoreUnique( SemaphoreCreateFlags flags ) -> UniqueSemaphore
  {
    SemaphoreCreateInfo createInfo( flags );

    UniqueSemaphore semaphore = RAYEX_NAMESPACE::g_device.createSemaphoreUnique( createInfo );
    RX_ASSERT( semaphore, "Failed to create semaphore." );

    return semaphore;
  }

  auto initSemaphore( SemaphoreCreateFlags flags ) -> Semaphore
  {
    SemaphoreCreateInfo createInfo( flags );

    Semaphore semaphore = RAYEX_NAMESPACE::g_device.createSemaphore( createInfo );
    RX_ASSERT( semaphore, "Failed to create semaphore." );

    return semaphore;
  }

  auto initCommandPoolUnique( uint32_t queueFamilyIndex, CommandPoolCreateFlags flags ) -> UniqueCommandPool
  {
    CommandPoolCreateInfo createInfo( flags, queueFamilyIndex );

    UniqueCommandPool commandPool = RAYEX_NAMESPACE::g_device.createCommandPoolUnique( createInfo );
    RX_ASSERT( commandPool, "Failed to create command pool." );

    return commandPool;
  }

  auto initCommandPool( uint32_t queueFamilyIndex, CommandPoolCreateFlags flags ) -> CommandPool
  {
    CommandPoolCreateInfo createInfo( flags, queueFamilyIndex );

    CommandPool commandPool = RAYEX_NAMESPACE::g_device.createCommandPool( createInfo );
    RX_ASSERT( commandPool, "Failed to create command pool." );

    return commandPool;
  }

  auto initDescriptorPoolUnique( const std::vector<DescriptorPoolSize>& poolSizes, uint32_t maxSets, DescriptorPoolCreateFlags flags ) -> UniqueDescriptorPool
  {
    DescriptorPoolCreateInfo createInfo( flags,                                      // flags
                                         maxSets,                                    // maxSets
                                         static_cast<uint32_t>( poolSizes.size( ) ), // poolSizeCount
                                         poolSizes.data( ) );                        // pPoolSizes

    UniqueDescriptorPool descriptorPool = RAYEX_NAMESPACE::g_device.createDescriptorPoolUnique( createInfo );
    RX_ASSERT( descriptorPool, "Failed to create descriptor pool." );

    return descriptorPool;
  }

  auto initDescriptorSetsUnique( const UniqueDescriptorPool& pool, const UniqueDescriptorSetLayout& layout ) -> std::vector<DescriptorSet>
  {
    RX_ASSERT( RAYEX_NAMESPACE::g_swapchainImageCount > 0, "Failed to init descriptor sets because swapchain image count is zero." );

    // Create as many sets as images in the swapchain.
    std::vector<DescriptorSetLayout> layouts( RAYEX_NAMESPACE::g_swapchainImageCount, layout.get( ) );

    DescriptorSetAllocateInfo allocInfo( pool.get( ),
                                         RAYEX_NAMESPACE::g_swapchainImageCount,
                                         layouts.data( ) );

    auto sets = RAYEX_NAMESPACE::g_device.allocateDescriptorSets( allocInfo );

    for ( const DescriptorSet& set : sets )
    {
      RX_ASSERT( set, "Failed to create descriptor sets." );
    }

    return sets;
  }

  auto allocateMemoryUnique( Image image, MemoryPropertyFlags propertyFlags, void* pNext ) -> UniqueDeviceMemory
  {
    auto memoryRequirements = RAYEX_NAMESPACE::g_device.getImageMemoryRequirements( image );

    MemoryAllocateInfo allocateInfo( memoryRequirements.size,                                                                                         // allocationSize
                                     Helper::findMemoryType( RAYEX_NAMESPACE::g_physicalDevice, memoryRequirements.memoryTypeBits, propertyFlags ) ); // memoryTypeIndex

    allocateInfo.pNext = pNext;

    UniqueDeviceMemory memory = RAYEX_NAMESPACE::g_device.allocateMemoryUnique( allocateInfo );
    RX_ASSERT( memory, "Failed to create memory for image." );

    RAYEX_NAMESPACE::g_device.bindImageMemory( image, memory.get( ), 0 );

    return memory;
  }

  auto allocateMemory( Image image, MemoryPropertyFlags propertyFlags, void* pNext ) -> DeviceMemory
  {
    auto memoryRequirements = RAYEX_NAMESPACE::g_device.getImageMemoryRequirements( image );

    MemoryAllocateInfo allocateInfo( memoryRequirements.size,                                                                                         // allocationSize
                                     Helper::findMemoryType( RAYEX_NAMESPACE::g_physicalDevice, memoryRequirements.memoryTypeBits, propertyFlags ) ); // memoryTypeIndex

    allocateInfo.pNext = pNext;

    DeviceMemory memory = RAYEX_NAMESPACE::g_device.allocateMemory( allocateInfo );
    RX_ASSERT( memory, "Failed to create memory for image." );

    RAYEX_NAMESPACE::g_device.bindImageMemory( image, memory, 0 );

    return memory;
  }

  auto allocateMemoryUnique( Buffer buffer, MemoryPropertyFlags propertyFlags, void* pNext ) -> UniqueDeviceMemory
  {
    auto memoryRequirements = RAYEX_NAMESPACE::g_device.getBufferMemoryRequirements( buffer );

    MemoryAllocateInfo allocateInfo( memoryRequirements.size,                                                                                         // allocationSize
                                     Helper::findMemoryType( RAYEX_NAMESPACE::g_physicalDevice, memoryRequirements.memoryTypeBits, propertyFlags ) ); // memoryTypeIndex

    allocateInfo.pNext = pNext;

    UniqueDeviceMemory memory = RAYEX_NAMESPACE::g_device.allocateMemoryUnique( allocateInfo );
    RX_ASSERT( memory, "Failed to create memory for image." );

    RAYEX_NAMESPACE::g_device.bindBufferMemory( buffer, memory.get( ), 0 );

    return memory;
    ;
  }

  void allocateMemory( RAYEX_NAMESPACE::AccelerationStructure& as )
  {
    AccelerationStructureMemoryRequirementsInfoKHR memInfo( AccelerationStructureMemoryRequirementsTypeKHR::eObject, // type
                                                            AccelerationStructureBuildTypeKHR::eDevice,              // buildType
                                                            as.as );                                                 // accelerationStructure

    MemoryRequirements2 memoryRequirements = RAYEX_NAMESPACE::g_device.getAccelerationStructureMemoryRequirementsKHR( memInfo );

    MemoryAllocateFlagsInfo allocateFlags( MemoryAllocateFlagBits::eDeviceAddress, // flags
                                           { } );                                  // deviceMask

    MemoryAllocateInfo allocateInfo( memoryRequirements.memoryRequirements.size,                                                                                                                // allocationSize
                                     Helper::findMemoryType( RAYEX_NAMESPACE::g_physicalDevice, memoryRequirements.memoryRequirements.memoryTypeBits, MemoryPropertyFlagBits::eDeviceLocal ) ); // memoryTypeIndex

    as.memory = RAYEX_NAMESPACE::g_device.allocateMemory( allocateInfo );
    RX_ASSERT( as.memory, "Failed to create memory for acceleration structure." );

    BindAccelerationStructureMemoryInfoKHR bindInfo( as.as,     // accelerationStructure
                                                     as.memory, // memory
                                                     0,         // memoryOffset
                                                     0,         // deviceIndexCount
                                                     nullptr ); // pDeviceIndices

    if ( RAYEX_NAMESPACE::g_device.bindAccelerationStructureMemoryKHR( 1, &bindInfo ) != Result::eSuccess )
    {
      RX_ERROR( "Failed to bind acceleration structure memory." );
    }
  }

  auto allocateMemory( Buffer buffer, MemoryPropertyFlags propertyFlags, void* pNext ) -> DeviceMemory
  {
    auto memoryRequirements = RAYEX_NAMESPACE::g_device.getBufferMemoryRequirements( buffer );

    MemoryAllocateInfo allocateInfo( memoryRequirements.size,                                                                                         // allocationSize
                                     Helper::findMemoryType( RAYEX_NAMESPACE::g_physicalDevice, memoryRequirements.memoryTypeBits, propertyFlags ) ); // memoryTypeIndex

    allocateInfo.pNext = pNext;

    DeviceMemory memory = RAYEX_NAMESPACE::g_device.allocateMemory( allocateInfo );
    RX_ASSERT( memory, "Failed to create memory for image." );

    RAYEX_NAMESPACE::g_device.bindBufferMemory( buffer, memory, 0 );

    return memory;
    ;
  }

  auto initImageViewUnique( Image image, Format format, ImageAspectFlags aspectFlags ) -> UniqueImageView
  {
    ComponentMapping components = {
      ComponentSwizzle::eIdentity,
      ComponentSwizzle::eIdentity,
      ComponentSwizzle::eIdentity,
      ComponentSwizzle::eIdentity
    };

    ImageSubresourceRange subresourceRange = {
      aspectFlags, // aspectMask
      0U,          // baseMipLevel
      1U,          // levelCount
      0U,          // baseArrayLayer
      1U           // layerCount
    };

    ImageViewCreateInfo createInfo( { },                // flags
                                    image,              // image
                                    ImageViewType::e2D, // viewType
                                    format,             // format
                                    components,         // components
                                    subresourceRange ); // subresourceRange

    UniqueImageView imageView = RAYEX_NAMESPACE::g_device.createImageViewUnique( createInfo );
    RX_ASSERT( imageView, "Failed to create image view." );

    return imageView;
  }

  auto initImageView( Image image, Format format, ImageAspectFlags aspectFlags ) -> ImageView
  {
    ComponentMapping components = {
      ComponentSwizzle::eIdentity,
      ComponentSwizzle::eIdentity,
      ComponentSwizzle::eIdentity,
      ComponentSwizzle::eIdentity
    };

    ImageSubresourceRange subresourceRange = {
      aspectFlags, // aspectMask
      0U,          // baseMipLevel
      1U,          // levelCount
      0U,          // baseArrayLayer
      1U           // layerCount
    };

    ImageViewCreateInfo createInfo( { },                // flags
                                    image,              // image
                                    ImageViewType::e2D, // viewType
                                    format,             // format
                                    components,         // components
                                    subresourceRange ); // subresourceRange

    ImageView imageView = RAYEX_NAMESPACE::g_device.createImageView( createInfo );
    RX_ASSERT( imageView, "Failed to create image view." );

    return imageView;
  }

  auto initSamplerUnique( const SamplerCreateInfo& createInfo ) -> UniqueSampler
  {
    UniqueSampler sampler = RAYEX_NAMESPACE::g_device.createSamplerUnique( createInfo );
    RX_ASSERT( sampler, "Failed to create sampler." );

    return sampler;
  }

  auto initSampler( const SamplerCreateInfo& createInfo ) -> Sampler
  {
    Sampler sampler = RAYEX_NAMESPACE::g_device.createSampler( createInfo );
    RX_ASSERT( sampler, "Failed to create sampler." );

    return sampler;
  }

  auto initFramebufferUnique( const std::vector<ImageView>& attachments, RenderPass renderPass, const Extent2D& extent ) -> UniqueFramebuffer
  {
    FramebufferCreateInfo createInfo( { },                                          // flags
                                      renderPass,                                   // renderPass
                                      static_cast<uint32_t>( attachments.size( ) ), // attachmentCount
                                      attachments.data( ),                          // pAttachments
                                      extent.width,                                 // width
                                      extent.height,                                // height
                                      1U );                                         // layers

    UniqueFramebuffer framebuffer = RAYEX_NAMESPACE::g_device.createFramebufferUnique( createInfo );
    RX_ASSERT( framebuffer, "Failed to create framebuffer." );

    return framebuffer;
  }

  auto initFramebuffer( const std::vector<ImageView>& attachments, RenderPass renderPass, const Extent2D& extent ) -> Framebuffer
  {
    FramebufferCreateInfo createInfo( { },                                          // flags
                                      renderPass,                                   // renderPass
                                      static_cast<uint32_t>( attachments.size( ) ), // attachmentCount
                                      attachments.data( ),                          // pAttachments
                                      extent.width,                                 // width
                                      extent.height,                                // height
                                      1U );                                         // layers

    Framebuffer framebuffer = RAYEX_NAMESPACE::g_device.createFramebuffer( createInfo );
    RX_ASSERT( framebuffer, "Failed to create framebuffer." );

    return framebuffer;
  }

  auto initQueryPoolUnique( uint32_t count, QueryType type ) -> UniqueQueryPool
  {
    QueryPoolCreateInfo createInfo( { },   // flags
                                    type,  // queryType
                                    count, // queryCount
                                    { } ); // pipelineStatistics

    UniqueQueryPool queryPool = RAYEX_NAMESPACE::g_device.createQueryPoolUnique( createInfo );
    RX_ASSERT( queryPool, "Failed to create query pool." );

    return queryPool;
  }

  auto initQueryPool( uint32_t count, QueryType type ) -> QueryPool
  {
    QueryPoolCreateInfo createInfo( { },   // flags
                                    type,  // queryType
                                    count, // queryCount
                                    { } ); // pipelineStatistics

    QueryPool queryPool = RAYEX_NAMESPACE::g_device.createQueryPool( createInfo );
    RX_ASSERT( queryPool, "Failed to create query pool." );

    return queryPool;
  }

  auto initShaderModuleUnique( std::string_view path ) -> UniqueShaderModule
  {
    std::vector<char> source = RAYEX_NAMESPACE::Util::parseShader( path );

    ShaderModuleCreateInfo createInfo( { },                                                   // flags
                                       source.size( ),                                        // codeSize
                                       reinterpret_cast<const uint32_t*>( source.data( ) ) ); // pCode

    UniqueShaderModule shaderModule = RAYEX_NAMESPACE::g_device.createShaderModuleUnique( createInfo );
    RX_ASSERT( shaderModule, "Failed to create shader module." );

    return shaderModule;
  }

  auto initShaderModule( std::string_view path ) -> ShaderModule
  {
    std::vector<char> source = RAYEX_NAMESPACE::Util::parseShader( path );

    ShaderModuleCreateInfo createInfo( { },                                                   // flags
                                       source.size( ),                                        // codeSize
                                       reinterpret_cast<const uint32_t*>( source.data( ) ) ); // pCode

    ShaderModule shaderModule = RAYEX_NAMESPACE::g_device.createShaderModule( createInfo );
    RX_ASSERT( shaderModule, "Failed to create shader module." );

    return shaderModule;
  }

  auto initAccelerationStructure( const AccelerationStructureCreateInfoKHR& asCreateInfo ) -> RAYEX_NAMESPACE::AccelerationStructure
  {
    RAYEX_NAMESPACE::AccelerationStructure resultAs;
    resultAs.as = RAYEX_NAMESPACE::g_device.createAccelerationStructureKHR( asCreateInfo, nullptr );

    allocateMemory( resultAs );

    return resultAs;
  }

  auto initPhysicalDevice( ) -> PhysicalDevice
  {
    PhysicalDevice physicalDevice;

    auto physicalDevices = RAYEX_NAMESPACE::g_instance.enumeratePhysicalDevices( );

    std::vector<std::pair<unsigned int, std::string>> results;

    unsigned int score = 0;
    for ( const auto& it : physicalDevices )
    {
      auto temp = Helper::evaluatePhysicalDevice( it );
      results.push_back( temp );

      if ( temp.first > score )
      {
        physicalDevice = it;
        score          = temp.first;
      }
    }

    // Print information about all GPUs available on the machine.
    const std::string separator = "===================================================================";
    std::cout << "Physical device report: "
              << "\n"
              << separator << "\n"
              << "Device name"
              << "\t\t\t"
              << "Score" << std::endl
              << separator << "\n";

    for ( const auto& result : results )
    {
      std::cout << std::left << std::setw( 32 ) << std::setfill( ' ' ) << result.second << std::left << std::setw( 32 ) << std::setfill( ' ' ) << result.first << std::endl;
    }

    RX_ASSERT( physicalDevice, "No suitable physical device was found." );

    // Print information about the GPU that was selected.
    auto properties = physicalDevice.getProperties( );
    RX_SUCCESS( "Selected GPU: ", properties.deviceName );

    RAYEX_NAMESPACE::g_physicalDeviceLimits = properties.limits;
    RAYEX_NAMESPACE::g_physicalDevice       = physicalDevice;

    return physicalDevice;
  }

  void initQueueFamilyIndices( )
  {
    std::optional<uint32_t> graphicsFamilyIndex;
    std::optional<uint32_t> transferFamilyIndex;

    auto queueFamilyProperties = RAYEX_NAMESPACE::g_physicalDevice.getQueueFamilyProperties( );
    std::vector<uint32_t> queueFamilies( queueFamilyProperties.size( ) );

    bool dedicatedTransferQueueFamily = Helper::isPhysicalDeviceWithDedicatedTransferQueueFamily( RAYEX_NAMESPACE::g_physicalDevice );

    for ( uint32_t index = 0; index < static_cast<uint32_t>( queueFamilies.size( ) ); ++index )
    {
      if ( queueFamilyProperties[index].queueFlags & QueueFlagBits::eGraphics && !graphicsFamilyIndex.has_value( ) )
      {
        if ( RAYEX_NAMESPACE::g_physicalDevice.getSurfaceSupportKHR( index, RAYEX_NAMESPACE::g_surface ) )
        {
          graphicsFamilyIndex = index;
        }
      }

      if ( dedicatedTransferQueueFamily )
      {
        if ( !( queueFamilyProperties[index].queueFlags & QueueFlagBits::eGraphics ) && !transferFamilyIndex.has_value( ) )
        {
          transferFamilyIndex = index;
        }
      }
      else
      {
        if ( queueFamilyProperties[index].queueFlags & QueueFlagBits::eTransfer && !transferFamilyIndex.has_value( ) )
        {
          transferFamilyIndex = index;
        }
      }
    }

    if ( !graphicsFamilyIndex.has_value( ) || !transferFamilyIndex.has_value( ) )
    {
      RX_FATAL( "Failed to retrieve queue family indices." );
    }

    RAYEX_NAMESPACE::g_graphicsFamilyIndex = graphicsFamilyIndex.value( );
    RAYEX_NAMESPACE::g_transferFamilyIndex = transferFamilyIndex.value( );
  }

  auto initDevice( std::vector<const char*>& extensions ) -> UniqueDevice
  {
    Helper::checkDeviceExtensionSupport( extensions );

    std::vector<DeviceQueueCreateInfo> queueCreateInfos;

    const float queuePriority = 1.0F;

    std::vector<uint32_t> queueFamilyIndices = { RAYEX_NAMESPACE::g_graphicsFamilyIndex, RAYEX_NAMESPACE::g_transferFamilyIndex };

    uint32_t index = 0;
    for ( const auto& queueFamilyIndex : queueFamilyIndices )
    {
      DeviceQueueCreateInfo queueCreateInfo( { },              // flags
                                             queueFamilyIndex, // queueFamilyIndex
                                             1,                // queueCount
                                             &queuePriority ); // pQueuePriorties

      queueCreateInfos.push_back( queueCreateInfo );

      ++index;
    }

    PhysicalDeviceDescriptorIndexingFeatures indexingFeatures;
    indexingFeatures.runtimeDescriptorArray                     = VK_TRUE;
    indexingFeatures.shaderStorageBufferArrayNonUniformIndexing = VK_TRUE;
    indexingFeatures.descriptorBindingVariableDescriptorCount   = VK_TRUE;
    indexingFeatures.descriptorBindingPartiallyBound            = VK_TRUE;

    PhysicalDeviceRobustness2FeaturesEXT robustness2FeaturesEXT;
    robustness2FeaturesEXT.nullDescriptor = VK_TRUE;
    robustness2FeaturesEXT.pNext          = &indexingFeatures;

    PhysicalDeviceRayTracingFeaturesKHR rayTracingFeatures;
    rayTracingFeatures.rayTracing = VK_TRUE;
    rayTracingFeatures.rayQuery   = VK_TRUE;
    rayTracingFeatures.pNext      = &robustness2FeaturesEXT;

    PhysicalDeviceBufferDeviceAddressFeatures bufferDeviceAddressFeatures;
    bufferDeviceAddressFeatures.bufferDeviceAddress = VK_TRUE;
    bufferDeviceAddressFeatures.pNext               = &rayTracingFeatures;

    PhysicalDeviceFeatures deviceFeatures;
    deviceFeatures.samplerAnisotropy              = VK_TRUE;
    deviceFeatures.vertexPipelineStoresAndAtomics = VK_TRUE;

    PhysicalDeviceFeatures2 deviceFeatures2 { deviceFeatures };
    deviceFeatures2.pNext = &bufferDeviceAddressFeatures;

    DeviceCreateInfo createInfo( { },                                               // flags
                                 static_cast<uint32_t>( queueCreateInfos.size( ) ), // queueCreateInfoCount
                                 queueCreateInfos.data( ),                          // pQueueCreateInfos
                                 0,                                                 // enabledLayerCount
                                 nullptr,                                           // ppEnabledLayerNames
                                 static_cast<uint32_t>( extensions.size( ) ),       // enabledExtensionCount
                                 extensions.data( ),                                // ppEnabledExtensionNames
                                 nullptr );                                         // pEnabledFeatures - must be NULL because the VkDeviceCreateInfo::pNext chain includes VkPhysicalDeviceFeatures2.

    createInfo.pNext = &deviceFeatures2;

    UniqueDevice device       = RAYEX_NAMESPACE::g_physicalDevice.createDeviceUnique( createInfo );
    RAYEX_NAMESPACE::g_device = device.get( );
    RX_ASSERT( device, "Failed to create logical device." );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( device.get( ) );

    return std::move( device );
  }

  auto initInstance( const std::vector<const char*>& layers, std::vector<const char*>& extensions ) -> UniqueInstance
  {
    UniqueInstance instance;

    DynamicLoader dl;
    auto vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( vkGetInstanceProcAddr );

    // Retrieve all extensions needed by SDL2.
    uint32_t sdlExtensionsCount;
    SDL_bool result = SDL_Vulkan_GetInstanceExtensions( RAYEX_NAMESPACE::g_window->get( ), &sdlExtensionsCount, nullptr );

    if ( result != SDL_TRUE )
    {
      RX_ERROR( "Failed to get extensions required by SDL." );
    }

    gsl::owner<const char**> sdlExtensionsNames = new const char*[sdlExtensionsCount];
    result                                      = SDL_Vulkan_GetInstanceExtensions( RAYEX_NAMESPACE::g_window->get( ), &sdlExtensionsCount, sdlExtensionsNames );

    if ( result != SDL_TRUE )
    {
      RX_ERROR( "Failed to get extensions required by SDL." );
    }

    gsl::span<const char*> windowExtensions( sdlExtensionsNames, sdlExtensionsCount );
    extensions.insert( extensions.end( ), windowExtensions.begin( ), windowExtensions.end( ) );

    // Check if all extensions and layers needed are available.
    Helper::checkInstanceLayersSupport( layers );
    Helper::checkInstanceExtensionsSupport( extensions );

    // Start creating the instance.
    ApplicationInfo appInfo;

    uint32_t apiVersion = enumerateInstanceVersion( );

#if defined( VK_API_VERSION_1_1 ) && !defined( VK_API_VERSION_1_2 )
    if ( apiVersion >= VK_API_VERSION_1_1 )
    {
      RX_SUCCESS( "Found Vulkan SDK API version 1.1" );
      appInfo.apiVersion = VK_API_VERSION_1_1;
    }
    else
    {
      RX_FATAL( "This application requires Vulkan SDK API Version 1.1 or higher." );
    }
#endif

#if defined( VK_API_VERSION_1_2 )
    if ( apiVersion >= VK_API_VERSION_1_2 )
    {
      RX_SUCCESS( "Found Vulkan SDK API version 1.2" );
      appInfo.apiVersion = VK_API_VERSION_1_2;
    }
    else
    {
      RX_ERROR( "This application requires Vulkan SDK API Version 1.1 or higher." );
    }
#endif

    InstanceCreateInfo createInfo( { },                                         // flags
                                   &appInfo,                                    // pApplicationInfo
                                   static_cast<uint32_t>( layers.size( ) ),     // enabledLayerCount
                                   layers.data( ),                              // ppEnabledLayerNames
                                   static_cast<uint32_t>( extensions.size( ) ), // enabledExtensionCount
                                   extensions.data( ) );                        // ppEnabledExtensionNames

    instance                    = createInstanceUnique( createInfo );
    RAYEX_NAMESPACE::g_instance = instance.get( );
    RX_ASSERT( instance, "Failed to create instance." );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( instance.get( ) );

    return std::move( instance );
  }

  /*
  auto initGraphicsPipelinesUnique( const std::vector<GraphicsPipelineCreateInfo>& createInfos ) -> std::vector<UniquePipeline>
  {
    vk::Result result;
    std::vector<vk::Pipeline> temp;
    std::tie( result, temp ) = RAYEX_NAMESPACE::g_device.createGraphicsPipelines( nullptr, createInfos );

    std::vector<vk::UniquePipeline> pipelines( temp.size( ) );

    if ( result != Result::eSuccess )
    {
      RX_FATAL( "Failed to create graphics pipelines." );
    }

    for ( size_t i = 0; i < temp.size( ); ++i )
    {
      if ( !temp[i] )
      {
        RX_FATAL( "Failed to create graphics pipeline ", i + 1, "/", temp.size( ), "." );
      }
      else
      {
        pipelines[i].reset( temp[i] );
      }
    }

    return pipelines;
  }

  auto initRayTracingPipelinesUnique( const std::vector<RayTracingPipelineCreateInfoKHR>& createInfos ) -> std::vector<UniquePipeline>
  {
    vk::Result result;
    std::vector<vk::Pipeline> temp;
    std::tie( result, temp ) = RAYEX_NAMESPACE::g_device.createRayTracingPipelinesKHR( nullptr, createInfos );

    std::vector<vk::UniquePipeline> pipelines( temp.size( ) );

    if ( result != Result::eSuccess )
    {
      RX_FATAL( "Failed to create graphics pipelines." );
    }

    for ( size_t i = 0; i < temp.size( ); ++i )
    {
      if ( !temp[i] )
      {
        RX_FATAL( "Failed to create graphics pipeline ", i + 1, "/", temp.size( ), "." );
      }
      else
      {
        pipelines[i].reset( temp[i] );
      }
    }

    return pipelines;
  }
  */
} // namespace vk::Initializer
