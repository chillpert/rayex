#include "api/utility/Initializers.hpp"
#include "api/utility/Helpers.hpp"
#include "api/utility/Util.hpp"
#include "api/buffers/CommandBuffer.hpp"
#include "api/misc/Vertex.hpp"

#define VULKAN_HPP_STORAGE_SHARED
#define VULKAN_HPP_STORAGE_SHARED_EXPORT
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace vk
{
  namespace Initializer
  {
    UniqueFence initFenceUnique( FenceCreateFlags flags )
    {
      FenceCreateInfo createInfo( flags );

      UniqueFence fence = RAYEXEC_NAMESPACE::g_device.createFenceUnique( createInfo );
      RX_ASSERT( fence, "Failed to create fence." );

      return fence;
    }

    Fence initFence( FenceCreateFlags flags )
    {
      FenceCreateInfo createInfo( flags );

      Fence fence = RAYEXEC_NAMESPACE::g_device.createFence( createInfo );
      RX_ASSERT( fence, "Failed to create fence." );

      return fence;
    }

    UniqueSemaphore initSemaphoreUnique( SemaphoreCreateFlags flags )
    {
      SemaphoreCreateInfo createInfo( flags );

      UniqueSemaphore semaphore = RAYEXEC_NAMESPACE::g_device.createSemaphoreUnique( createInfo );
      RX_ASSERT( semaphore, "Failed to create semaphore." );

      return semaphore;
    }

    Semaphore initSemaphore( SemaphoreCreateFlags flags )
    {
      SemaphoreCreateInfo createInfo( flags );

      Semaphore semaphore = RAYEXEC_NAMESPACE::g_device.createSemaphore( createInfo );
      RX_ASSERT( semaphore, "Failed to create semaphore." );

      return semaphore;
    }

    UniqueCommandPool initCommandPoolUnique( uint32_t queueFamilyIndex, CommandPoolCreateFlags flags )
    {
      CommandPoolCreateInfo createInfo( flags, queueFamilyIndex );

      UniqueCommandPool commandPool = RAYEXEC_NAMESPACE::g_device.createCommandPoolUnique( createInfo );
      RX_ASSERT( commandPool, "Failed to create command pool." );

      return commandPool;
    }

    CommandPool initCommandPool( uint32_t queueFamilyIndex, CommandPoolCreateFlags flags )
    {
      CommandPoolCreateInfo createInfo( flags, queueFamilyIndex );

      CommandPool commandPool = RAYEXEC_NAMESPACE::g_device.createCommandPool( createInfo );
      RX_ASSERT( commandPool, "Failed to create command pool." );

      return commandPool;
    }

    UniqueDescriptorPool initDescriptorPoolUnique( const std::vector<DescriptorSetLayoutBinding>& layoutBindings, uint32_t maxSets, DescriptorPoolCreateFlags flags )
    {
      auto poolSizes = Helper::getPoolSizes( layoutBindings, maxSets );

      DescriptorPoolCreateInfo createInfo( flags,                                      // flags
                                           maxSets,                                    // maxSets
                                           static_cast<uint32_t>( poolSizes.size( ) ), // poolSizeCount
                                           poolSizes.data( ) );                        // pPoolSizes

      UniqueDescriptorPool descriptorPool = RAYEXEC_NAMESPACE::g_device.createDescriptorPoolUnique( createInfo );
      RX_ASSERT( descriptorPool, "Failed to create descriptor pool." );

      return descriptorPool;
    }

    UniqueDescriptorPool initDescriptorPoolUnique( const std::vector<DescriptorPoolSize>& poolSizes, uint32_t maxSets, DescriptorPoolCreateFlags flags )
    {
      DescriptorPoolCreateInfo createInfo( flags,                                      // flags
                                           maxSets,                                    // maxSets
                                           static_cast<uint32_t>( poolSizes.size( ) ), // poolSizeCount
                                           poolSizes.data( ) );                        // pPoolSizes

      UniqueDescriptorPool descriptorPool = RAYEXEC_NAMESPACE::g_device.createDescriptorPoolUnique( createInfo );
      RX_ASSERT( descriptorPool, "Failed to create descriptor pool." );

      return descriptorPool;
    }

    DescriptorPool initDescriptorPool( const std::vector<DescriptorSetLayoutBinding>& layoutBindings, uint32_t maxSets, DescriptorPoolCreateFlags flags )
    {
      auto poolSizes = Helper::getPoolSizes( layoutBindings, maxSets );

      DescriptorPoolCreateInfo createInfo( flags,                                        // flags
                                           maxSets,                                      // maxSets
                                           static_cast<uint32_t>( poolSizes.size( ) ), // poolSizeCount
                                           poolSizes.data( ) );                          // pPoolSizes

      DescriptorPool descriptorPool = RAYEXEC_NAMESPACE::g_device.createDescriptorPool( createInfo );
      RX_ASSERT( descriptorPool, "Failed to create descriptor pool." );

      return descriptorPool;
    }

    DescriptorSetLayout initDescriptorSetLayout( const std::vector<DescriptorSetLayoutBinding> bindings )
    {
      DescriptorSetLayoutCreateInfo createInfo( { },                                       // flags
                                                    static_cast<uint32_t>( bindings.size( ) ), // bindingCount
                                                    bindings.data( ) );                        // pBindings

      auto descriptorSetLayout = RAYEXEC_NAMESPACE::g_device.createDescriptorSetLayout( createInfo );
      RX_ASSERT( descriptorSetLayout, "Failed to create descriptor set layout." );

      return descriptorSetLayout;
    }

    std::vector<DescriptorSet> initDescriptorSets( DescriptorPool pool, DescriptorSetLayout layout )
    {
      std::vector<DescriptorSetLayout> temp { RAYEXEC_NAMESPACE::g_swapchainImageCount, layout };

      DescriptorSetAllocateInfo allocInfo( pool,
                                               RAYEXEC_NAMESPACE::g_swapchainImageCount,
                                               temp.data( ) );

      auto sets = RAYEXEC_NAMESPACE::g_device.allocateDescriptorSets( allocInfo );

      for ( const auto& set : sets )
        RX_ASSERT( set, "Failed to create descriptor sets." );

      return sets;
    }

    std::vector<UniqueDescriptorSet> initDescriptorSetsUnique( DescriptorPool pool, DescriptorSetLayout layout )
    {
      std::vector<DescriptorSetLayout> temp { RAYEXEC_NAMESPACE::g_swapchainImageCount, layout };

      DescriptorSetAllocateInfo allocInfo( pool,
                                               RAYEXEC_NAMESPACE::g_swapchainImageCount,
                                               temp.data( ) );

      auto sets = RAYEXEC_NAMESPACE::g_device.allocateDescriptorSetsUnique( allocInfo );

      for ( const auto& set : sets )
        RX_ASSERT( set, "Failed to create descriptor sets." );

      return sets;
    }

    UniqueDescriptorSetLayout initDescriptorSetLayoutUnique( const std::vector<DescriptorSetLayoutBinding> bindings )
    {
      DescriptorSetLayoutCreateInfo createInfo( { },                                       // flags
                                                    static_cast<uint32_t>( bindings.size( ) ), // bindingCount
                                                    bindings.data( ) );                        // pBindings

      auto descriptorSetLayout = RAYEXEC_NAMESPACE::g_device.createDescriptorSetLayoutUnique( createInfo );
      RX_ASSERT( descriptorSetLayout, "Failed to create descriptor set layout." );

      return descriptorSetLayout;
    }

    UniqueDeviceMemory allocateMemoryUnique( Image image, MemoryPropertyFlags propertyFlags, void* pNext )
    {
      auto memoryRequirements = RAYEXEC_NAMESPACE::g_device.getImageMemoryRequirements( image );

      MemoryAllocateInfo allocateInfo( memoryRequirements.size,                                                                             // allocationSize 
                                       Helper::findMemoryType( RAYEXEC_NAMESPACE::g_physicalDevice, memoryRequirements.memoryTypeBits, propertyFlags ) );  // memoryTypeIndex

      allocateInfo.pNext = pNext;

      UniqueDeviceMemory memory = RAYEXEC_NAMESPACE::g_device.allocateMemoryUnique( allocateInfo );
      RX_ASSERT( memory, "Failed to create memory for image." );

      RAYEXEC_NAMESPACE::g_device.bindImageMemory( image, memory.get( ), 0 );

      return memory;
    }

    DeviceMemory allocateMemory( Image image, MemoryPropertyFlags propertyFlags, void* pNext )
    {
      auto memoryRequirements = RAYEXEC_NAMESPACE::g_device.getImageMemoryRequirements( image );

      MemoryAllocateInfo allocateInfo( memoryRequirements.size,                                                                             // allocationSize 
                                       Helper::findMemoryType( RAYEXEC_NAMESPACE::g_physicalDevice, memoryRequirements.memoryTypeBits, propertyFlags ) );  // memoryTypeIndex

      allocateInfo.pNext = pNext;

      DeviceMemory memory = RAYEXEC_NAMESPACE::g_device.allocateMemory( allocateInfo );
      RX_ASSERT( memory, "Failed to create memory for image." );

      RAYEXEC_NAMESPACE::g_device.bindImageMemory( image, memory, 0 );

      return memory;
    }

    UniqueDeviceMemory allocateMemoryUnique( Buffer buffer, MemoryPropertyFlags propertyFlags, void* pNext )
    {
      auto memoryRequirements = RAYEXEC_NAMESPACE::g_device.getBufferMemoryRequirements( buffer );

      MemoryAllocateInfo allocateInfo( memoryRequirements.size,                                                                             // allocationSize 
                                       Helper::findMemoryType( RAYEXEC_NAMESPACE::g_physicalDevice, memoryRequirements.memoryTypeBits, propertyFlags ) );  // memoryTypeIndex

      allocateInfo.pNext = pNext;

      UniqueDeviceMemory memory = RAYEXEC_NAMESPACE::g_device.allocateMemoryUnique( allocateInfo );
      RX_ASSERT( memory, "Failed to create memory for image." );

      RAYEXEC_NAMESPACE::g_device.bindBufferMemory( buffer, memory.get( ), 0 );

      return memory;;
    }

    void allocateMemory( RAYEXEC_NAMESPACE::AccelerationStructure& as )
    {
      AccelerationStructureMemoryRequirementsInfoKHR memInfo( AccelerationStructureMemoryRequirementsTypeKHR::eObject, // type
                                                              AccelerationStructureBuildTypeKHR::eDevice,              // buildType
                                                              as.as );                                                 // accelerationStructure

      MemoryRequirements2 memoryRequirements = RAYEXEC_NAMESPACE::g_device.getAccelerationStructureMemoryRequirementsKHR( memInfo );

      MemoryAllocateFlagsInfo allocateFlags( MemoryAllocateFlagBits::eDeviceAddress, // flags
                                             { } );                              // deviceMask

      MemoryAllocateInfo allocateInfo( memoryRequirements.memoryRequirements.size,                                                                                                   // allocationSize
                                       Helper::findMemoryType( RAYEXEC_NAMESPACE::g_physicalDevice, memoryRequirements.memoryRequirements.memoryTypeBits, MemoryPropertyFlagBits::eDeviceLocal ) ); // memoryTypeIndex

      as.memory = RAYEXEC_NAMESPACE::g_device.allocateMemory( allocateInfo );
      RX_ASSERT( as.memory, "Failed to create memory for acceleration structure." );

      BindAccelerationStructureMemoryInfoKHR bindInfo( as.as,     // accelerationStructure
                                                       as.memory, // memory
                                                       0,         // memoryOffset
                                                       0,         // deviceIndexCount
                                                       nullptr ); // pDeviceIndices

      if ( RAYEXEC_NAMESPACE::g_device.bindAccelerationStructureMemoryKHR( 1, &bindInfo ) != Result::eSuccess )
        RX_ERROR( "Failed to bind acceleration structure memory." );
    }

    DeviceMemory allocateMemory( Buffer buffer, MemoryPropertyFlags propertyFlags, void* pNext )
    {
      auto memoryRequirements = RAYEXEC_NAMESPACE::g_device.getBufferMemoryRequirements( buffer );

      MemoryAllocateInfo allocateInfo( memoryRequirements.size,                                                                             // allocationSize 
                                       Helper::findMemoryType( RAYEXEC_NAMESPACE::g_physicalDevice, memoryRequirements.memoryTypeBits, propertyFlags ) );  // memoryTypeIndex

      allocateInfo.pNext = pNext;

      DeviceMemory memory = RAYEXEC_NAMESPACE::g_device.allocateMemory( allocateInfo );
      RX_ASSERT( memory, "Failed to create memory for image." );

      RAYEXEC_NAMESPACE::g_device.bindBufferMemory( buffer, memory, 0 );

      return memory;;
    }

    UniqueImageView initImageViewUnique( Image image, Format format, ImageAspectFlags aspectFlags )
    {
      ComponentMapping components =
      {
        ComponentSwizzle::eIdentity,
        ComponentSwizzle::eIdentity,
        ComponentSwizzle::eIdentity,
        ComponentSwizzle::eIdentity
      };

      ImageSubresourceRange subresourceRange =
      {
        aspectFlags,  // aspectMask
        0u,           // baseMipLevel
        1u,           // levelCount
        0u,           // baseArrayLayer
        1u            // layerCount
      };

      ImageViewCreateInfo createInfo( { },                // flags
                                      image,              // image
                                      ImageViewType::e2D, // viewType
                                      format,             // format
                                      components,         // components
                                      subresourceRange ); // subresourceRange

      UniqueImageView imageView = RAYEXEC_NAMESPACE::g_device.createImageViewUnique( createInfo );
      RX_ASSERT( imageView, "Failed to create image view." );

      return imageView;
    }

    ImageView initImageView( Image image, Format format, ImageAspectFlags aspectFlags )
    {
      ComponentMapping components =
      {
        ComponentSwizzle::eIdentity,
        ComponentSwizzle::eIdentity,
        ComponentSwizzle::eIdentity,
        ComponentSwizzle::eIdentity
      };

      ImageSubresourceRange subresourceRange =
      {
        aspectFlags,  // aspectMask
        0u,           // baseMipLevel
        1u,           // levelCount
        0u,           // baseArrayLayer
        1u            // layerCount
      };

      ImageViewCreateInfo createInfo( { },                // flags
                                      image,              // image
                                      ImageViewType::e2D, // viewType
                                      format,             // format
                                      components,         // components
                                      subresourceRange ); // subresourceRange

      ImageView imageView = RAYEXEC_NAMESPACE::g_device.createImageView( createInfo );
      RX_ASSERT( imageView, "Failed to create image view." );

      return imageView;
    }

    UniqueSampler initSamplerUnique( const SamplerCreateInfo& createInfo )
    {
      UniqueSampler sampler = RAYEXEC_NAMESPACE::g_device.createSamplerUnique( createInfo );
      RX_ASSERT( sampler, "Failed to create sampler." );

      return sampler;
    }

    Sampler initSampler( const SamplerCreateInfo& createInfo )
    {
      Sampler sampler = RAYEXEC_NAMESPACE::g_device.createSampler( createInfo );
      RX_ASSERT( sampler, "Failed to create sampler." );

      return sampler;
    }

    UniqueFramebuffer initFramebufferUnique( const std::vector<ImageView>& attachments, RenderPass renderPass, const Extent2D& extent )
    {
      FramebufferCreateInfo createInfo( { },                                            // flags
                                        renderPass,                                     // renderPass
                                        static_cast<uint32_t>( attachments.size( ) ), // attachmentCount
                                        attachments.data( ),                            // pAttachments
                                        extent.width,                                   // width
                                        extent.height,                                  // height
                                        1u );                                           // layers

      UniqueFramebuffer framebuffer = RAYEXEC_NAMESPACE::g_device.createFramebufferUnique( createInfo );
      RX_ASSERT( framebuffer, "Failed to create framebuffer." );

      return framebuffer;
    }

    Framebuffer initFramebuffer( const std::vector<ImageView>& attachments, RenderPass renderPass, const Extent2D& extent )
    {
      FramebufferCreateInfo createInfo( { },                                        // flags
                                        renderPass,                                 // renderPass
                                        static_cast<uint32_t>( attachments.size( ) ),  // attachmentCount
                                        attachments.data( ),                         // pAttachments
                                        extent.width,                               // width
                                        extent.height,                              // height
                                        1u );                                       // layers

      Framebuffer framebuffer = RAYEXEC_NAMESPACE::g_device.createFramebuffer( createInfo );
      RX_ASSERT( framebuffer, "Failed to create framebuffer." );

      return framebuffer;
    }

    UniqueQueryPool initQueryPoolUnique( uint32_t count, QueryType type )
    {
      QueryPoolCreateInfo createInfo( { },    // flags
                                      type,   // queryType
                                      count,  // queryCount
                                      { } );  // pipelineStatistics

      UniqueQueryPool queryPool = RAYEXEC_NAMESPACE::g_device.createQueryPoolUnique( createInfo );
      RX_ASSERT( queryPool, "Failed to create query pool." );

      return queryPool;
    }

    QueryPool initQueryPool( uint32_t count, QueryType type )
    {
      QueryPoolCreateInfo createInfo( { },    // flags
                                      type,   // queryType
                                      count,  // queryCount
                                      { } );  // pipelineStatistics

      QueryPool queryPool = RAYEXEC_NAMESPACE::g_device.createQueryPool( createInfo );
      RX_ASSERT( queryPool, "Failed to create query pool." );

      return queryPool;
    }

    UniqueShaderModule initShaderModuleUnique( const std::string& path )
    {
      std::vector<char> source = RAYEXEC_NAMESPACE::Util::parseShader( path );

      ShaderModuleCreateInfo createInfo( { },                                                     // flags
                                         source.size( ),                                          // codeSize
                                         reinterpret_cast<const uint32_t*>( source.data( ) ) ); // pCode

      UniqueShaderModule shaderModule = RAYEXEC_NAMESPACE::g_device.createShaderModuleUnique( createInfo );
      RX_ASSERT( shaderModule, "Failed to create shader module." );

      return shaderModule;
    }

    ShaderModule initShaderModule( const std::string& path )
    {
      std::vector<char> source = RAYEXEC_NAMESPACE::Util::parseShader( path );

      ShaderModuleCreateInfo createInfo( { },                                                     // flags
                                         source.size( ),                                          // codeSize
                                         reinterpret_cast<const uint32_t*>( source.data( ) ) ); // pCode

      ShaderModule shaderModule = RAYEXEC_NAMESPACE::g_device.createShaderModule( createInfo );
      RX_ASSERT( shaderModule, "Failed to create shader module." );

      return shaderModule;
    }

    RAYEXEC_NAMESPACE::AccelerationStructure initAccelerationStructure( const AccelerationStructureCreateInfoKHR& asCreateInfo )
    {
      RAYEXEC_NAMESPACE::AccelerationStructure resultAs;
      resultAs.as = RAYEXEC_NAMESPACE::g_device.createAccelerationStructureKHR( asCreateInfo, nullptr );

      allocateMemory( resultAs );

      return resultAs;
    }

    PhysicalDevice initPhysicalDevice( )
    {
      PhysicalDevice physicalDevice;

      auto physicalDevices = RAYEXEC_NAMESPACE::g_instance.enumeratePhysicalDevices( );

      std::vector<std::pair<unsigned int, std::string>> results;

      unsigned int score = 0;
      for ( const auto& it : physicalDevices )
      {
        auto temp = Helper::evaluatePhysicalDevice( it );
        results.push_back( temp );

        if ( temp.first > score )
        {
          physicalDevice = it;
          score = temp.first;
        }
      }

      // Print information about all GPUs available on the machine.
      const std::string separator = "===================================================================";
      std::cout << "Physical device report: " << "\n" << separator << "\n" << "Device name" << "\t\t\t" << "Score" << std::endl << separator << "\n";

      for ( const auto& result : results )
        std::cout << std::left << std::setw( 32 ) << std::setfill( ' ' ) << result.second << std::left << std::setw( 32 ) << std::setfill( ' ' ) << result.first << std::endl;

      RX_ASSERT( physicalDevice, "No suitable physical device was found." );
     
      // Print information about the GPU that was selected.
      auto properties = physicalDevice.getProperties( );
      RX_SUCCESS( "Selected GPU: ", properties.deviceName );

      RAYEXEC_NAMESPACE::g_physicalDeviceLimits = properties.limits;
      RAYEXEC_NAMESPACE::g_physicalDevice = physicalDevice;

      return physicalDevice;
    }

    void initQueueFamilyIndices( )
    {
      std::optional<uint32_t> graphicsFamilyIndex;
      std::optional<uint32_t> transferFamilyIndex;

      auto queueFamilyProperties = RAYEXEC_NAMESPACE::g_physicalDevice.getQueueFamilyProperties( );
      std::vector<uint32_t> queueFamilies( queueFamilyProperties.size( ) );

      bool dedicatedTransferQueueFamily = Helper::isPhysicalDeviceWithDedicatedTransferQueueFamily( RAYEXEC_NAMESPACE::g_physicalDevice );

      for ( uint32_t index = 0; index < static_cast<uint32_t>( queueFamilies.size( ) ); ++index )
      {
        if ( queueFamilyProperties[index].queueFlags & QueueFlagBits::eGraphics && !graphicsFamilyIndex.has_value( ) )
        {
          if ( RAYEXEC_NAMESPACE::g_physicalDevice.getSurfaceSupportKHR( index, RAYEXEC_NAMESPACE::g_surface ) )
            graphicsFamilyIndex = index;
        }

        if ( dedicatedTransferQueueFamily )
        {
          if ( !( queueFamilyProperties[index].queueFlags & QueueFlagBits::eGraphics ) && !transferFamilyIndex.has_value( ) )
            transferFamilyIndex = index;
        }
        else
        {
          if ( queueFamilyProperties[index].queueFlags & QueueFlagBits::eTransfer && !transferFamilyIndex.has_value( ) )
            transferFamilyIndex = index;
        }
      }

      if ( !graphicsFamilyIndex.has_value( ) || !transferFamilyIndex.has_value( ) )
        RX_FATAL( "Failed to retrieve queue family indices." );

      RAYEXEC_NAMESPACE::g_graphicsFamilyIndex = graphicsFamilyIndex.value( );
      RAYEXEC_NAMESPACE::g_transferFamilyIndex = transferFamilyIndex.value( );
    }

    UniqueDevice initDevice( std::vector<const char*>& extensions )
    {
      Helper::checkDeviceExtensionSupport( extensions );

      std::vector<DeviceQueueCreateInfo> queueCreateInfos;

      const float queuePriority = 1.0f;

      std::vector<uint32_t> queueFamilyIndices = { rx::g_graphicsFamilyIndex, rx::g_transferFamilyIndex };

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
      indexingFeatures.runtimeDescriptorArray = VK_TRUE;
      indexingFeatures.shaderStorageBufferArrayNonUniformIndexing = VK_TRUE;
      indexingFeatures.descriptorBindingVariableDescriptorCount = VK_TRUE;
      indexingFeatures.descriptorBindingPartiallyBound = VK_TRUE;

      PhysicalDeviceRobustness2FeaturesEXT robustness2FeaturesEXT;
      robustness2FeaturesEXT.nullDescriptor = VK_TRUE;
      robustness2FeaturesEXT.pNext = &indexingFeatures;

      PhysicalDeviceRayTracingFeaturesKHR rayTracingFeatures;
      rayTracingFeatures.rayTracing = VK_TRUE;
      rayTracingFeatures.rayQuery = VK_TRUE;
      rayTracingFeatures.pNext = &robustness2FeaturesEXT;

      PhysicalDeviceBufferDeviceAddressFeatures bufferDeviceAddressFeatures;
      bufferDeviceAddressFeatures.bufferDeviceAddress = VK_TRUE;
      bufferDeviceAddressFeatures.pNext = &rayTracingFeatures;

      PhysicalDeviceFeatures deviceFeatures;
      deviceFeatures.samplerAnisotropy = VK_TRUE;
      deviceFeatures.vertexPipelineStoresAndAtomics = VK_TRUE;

      PhysicalDeviceFeatures2 deviceFeatures2 { deviceFeatures };
      deviceFeatures2.pNext = &bufferDeviceAddressFeatures;

      DeviceCreateInfo createInfo( { },                                                   // flags
                                   static_cast<uint32_t>( queueCreateInfos.size( ) ), // queueCreateInfoCount
                                   queueCreateInfos.data( ),                          // pQueueCreateInfos
                                   0,                                                 // enabledLayerCount
                                   nullptr,                                           // ppEnabledLayerNames
                                   static_cast<uint32_t>( extensions.size( ) ),       // enabledExtensionCount
                                   extensions.data( ),                                // ppEnabledExtensionNames
                                   nullptr );                                         // pEnabledFeatures - must be NULL because the VkDeviceCreateInfo::pNext chain includes VkPhysicalDeviceFeatures2.

      createInfo.pNext = &deviceFeatures2;

      UniqueDevice device = rx::g_physicalDevice.createDeviceUnique( createInfo );
      rx::g_device = device.get( );
      RX_ASSERT( device, "Failed to create logical device." );
      VULKAN_HPP_DEFAULT_DISPATCHER.init( device.get( ) );
      
      return std::move( device );
    }

    UniqueInstance initInstance( const std::vector<const char*>& layers, std::vector<const char*>& extensions )
    {
      UniqueInstance instance;

      DynamicLoader dl;
      PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
      VULKAN_HPP_DEFAULT_DISPATCHER.init( vkGetInstanceProcAddr );

      // Retrieve all extensions needed by SDL2.
      std::vector<const char*> windowExtensions = rx::g_window->getInstanceExtensions( );
      extensions.insert( extensions.end( ), windowExtensions.begin( ), windowExtensions.end( ) );

      // Check if all extensions and layers needed are available.
      Helper::checkInstanceLayersSupport( layers );
      Helper::checkInstanceExtensionsSupport( extensions );

      // Start creating the instance.
      ApplicationInfo appInfo;

      uint32_t apiVersion = enumerateInstanceVersion( );

    #ifdef VK_API_VERSION_1_1
      if ( apiVersion >= VK_API_VERSION_1_1 )
      {
        RX_SUCCESS( "Found Vulkan SDK API version 1.1" );
        appInfo.apiVersion = VK_API_VERSION_1_1;
      }
      else
        RX_FATAL( "This application requires Vulkan SDK API Version 1.1 or higher." );
    #endif

    #ifdef VK_API_VERSION_1_2
      if ( apiVersion >= VK_API_VERSION_1_2 )
      {
        RX_SUCCESS( "Found Vulkan SDK API version 1.2" );
        appInfo.apiVersion = VK_API_VERSION_1_2;
      }
      else
        RX_ERROR( "This application requires Vulkan SDK API Version 1.1 or higher." );
    #endif

      InstanceCreateInfo createInfo( { },                                         // flags
                                     &appInfo,                                    // pApplicationInfo
                                     static_cast<uint32_t>( layers.size( ) ),     // enabledLayerCount
                                     layers.data( ),                              // ppEnabledLayerNames
                                     static_cast<uint32_t>( extensions.size( ) ), // enabledExtensionCount
                                     extensions.data( ) );                        // ppEnabledExtensionNames

      instance = createInstanceUnique( createInfo );
      rx::g_instance = instance.get( );
      RX_ASSERT( instance, "Failed to create instance." );
      VULKAN_HPP_DEFAULT_DISPATCHER.init( instance.get( ) );

      return std::move( instance );
    }

    bool initGraphicsPipelines( const std::vector<GraphicsPipelineCreateInfo>& createInfos )
    {
      auto pipelines = rx::g_device.createGraphicsPipelines( nullptr, createInfos );

      if ( pipelines.result != Result::eSuccess )
      {
        RX_ERROR( "Failed to create graphics pipelines." );
        return false;
      }

      for ( size_t i = 0; i < pipelines.value.size( ); ++i )
      {
        if ( !pipelines.value[i] )
        {
          RX_ERROR( "Failed to create graphics pipeline ", i + 1, "/", pipelines.value.size( ), "." );
          return false;
        }
      }

      return true;
    }

    bool initRayTracingPipelines( const std::vector<RayTracingPipelineCreateInfoKHR> createInfos )
    {
      auto pipelines = rx::g_device.createRayTracingPipelinesKHR( nullptr, createInfos );

      if ( pipelines.result != Result::eSuccess )
      {
        RX_ERROR( "Failed to create graphics pipelines." );
        return false;
      }

      for ( size_t i = 0; i < pipelines.value.size( ); ++i )
      {
        if ( !pipelines.value[i] )
        {
          RX_ERROR( "Failed to create graphics pipeline ", i + 1, "/", pipelines.value.size( ), "." );
          return false;
        }
      }

      return true;
    }

    bool initRayTracingPipelinesUnique( std::vector<vk::UniquePipeline>& pipelines, const std::vector<RayTracingPipelineCreateInfoKHR> createInfos )
    {
     //rx::g_device.createRayTracingPipelinesKHRUnique( nullptr, createInfos, nullptr );
      //auto pipeline2( std::move( pipelines2 ) );

      //auto result = rx::g_device.createRayTracingPipelinesKHRUnique( nullptr, createInfos );
      
      /*
      if ( pipelines.result != Result::eSuccess )
      {
        RX_ERROR( "Failed to create graphics pipelines." );
        return false;
      }

      for ( size_t i = 0; i < pipelines.value.size( ); ++i )
      {
        if ( !pipelines.value[i] )
        {
          RX_ERROR( "Failed to create graphics pipeline ", i + 1, "/", pipelines.value.size( ), "." );
          return false;
        }
      }
      */

      return true;
    }
  }
}
