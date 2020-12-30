#include "api/utility/Initializers.hpp"

#include "api/Components.hpp"
#include "api/Vertex.hpp"
#include "api/buffers/Buffer.hpp"
#include "api/buffers/CommandBuffer.hpp"
#include "api/utility/Helpers.hpp"
#include "api/utility/Util.hpp"

namespace vk::Initializer
{
  auto findMemoryType2( vk::PhysicalDevice physicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties ) -> uint32_t
  {
    static vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties( );

    for ( uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i )
    {
      if ( ( ( typeFilter & ( 1 << i ) ) != 0U ) && ( memoryProperties.memoryTypes[i].propertyFlags & properties ) == properties )
      {
        return i;
      }
    }

    throw std::runtime_error( "vkCore: Failed to find suitable memory type." );

    return 0U;
  }

  auto allocateBufferMemory2( vk::Buffer buffer, vk::MemoryPropertyFlags propertyFlags, void* pNext ) -> vk::DeviceMemory
  {
    auto memoryRequirements = rx::components::device.getBufferMemoryRequirements( buffer );

    vk::MemoryAllocateInfo allocateInfo( memoryRequirements.size,                                                                               // allocationSize
                                         findMemoryType2( rx::components::physicalDevice, memoryRequirements.memoryTypeBits, propertyFlags ) ); // memoryTypeIndex

    allocateInfo.pNext = pNext;

    auto memory = rx::components::device.allocateMemory( allocateInfo );
    VK_CORE_ASSERT( memory, "Failed to create memory for image." );

    rx::components::device.bindBufferMemory( buffer, memory, 0 );

    return memory;
  }

  auto initAccelerationStructure( AccelerationStructureCreateInfoKHR& asCreateInfo ) -> RAYEX_NAMESPACE::AccelerationStructure
  {
    RAYEX_NAMESPACE::AccelerationStructure resultAs;

    vk::MemoryAllocateFlagsInfo allocateFlags( vk::MemoryAllocateFlagBitsKHR::eDeviceAddress );

    vk::BufferCreateInfo createInfo( { },                                                                                                       // flags
                                     asCreateInfo.size,                                                                                         // size
                                     vk::BufferUsageFlagBits::eAccelerationStructureStorageKHR | vk::BufferUsageFlagBits::eShaderDeviceAddress, // usage
                                     vk::SharingMode::eExclusive,                                                                               // sharingMode
                                     1,                                                                                                         // queueFamilyIndexCount
                                     &rx::components::graphicsFamilyIndex );                                                                    // pQueueFamilyIndices

    resultAs.buffer = rx::components::device.createBuffer( createInfo );
    RX_ASSERT( resultAs.buffer, "Failed to create buffer." );

    resultAs.memory = allocateBufferMemory2( resultAs.buffer,
                                             vk::MemoryPropertyFlagBits::eDeviceLocal | vk::MemoryPropertyFlagBits::eHostCoherent,
                                             &allocateFlags );

    asCreateInfo.buffer = resultAs.buffer;

    resultAs.as = RAYEX_NAMESPACE::components::device.createAccelerationStructureKHR( asCreateInfo, nullptr );

    return resultAs;
  }

  auto initPhysicalDevice( ) -> PhysicalDevice
  {
    PhysicalDevice physicalDevice;

    auto physicalDevices = RAYEX_NAMESPACE::components::instance.enumeratePhysicalDevices( );

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

    RAYEX_NAMESPACE::components::physicalDeviceLimits = properties.limits;
    RAYEX_NAMESPACE::components::physicalDevice       = physicalDevice;

    return physicalDevice;
  }

  void initQueueFamilyIndices( )
  {
    std::optional<uint32_t> graphicsFamilyIndex;
    std::optional<uint32_t> transferFamilyIndex;

    auto queueFamilyProperties = RAYEX_NAMESPACE::components::physicalDevice.getQueueFamilyProperties( );
    std::vector<uint32_t> queueFamilies( queueFamilyProperties.size( ) );

    bool dedicatedTransferQueueFamily = Helper::isPhysicalDeviceWithDedicatedTransferQueueFamily( RAYEX_NAMESPACE::components::physicalDevice );

    for ( uint32_t index = 0; index < static_cast<uint32_t>( queueFamilies.size( ) ); ++index )
    {
      if ( queueFamilyProperties[index].queueFlags & QueueFlagBits::eGraphics && !graphicsFamilyIndex.has_value( ) )
      {
        if ( RAYEX_NAMESPACE::components::physicalDevice.getSurfaceSupportKHR( index, RAYEX_NAMESPACE::components::surface ) )
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

    RAYEX_NAMESPACE::components::graphicsFamilyIndex = graphicsFamilyIndex.value( );
    RAYEX_NAMESPACE::components::transferFamilyIndex = transferFamilyIndex.value( );
  }

  auto initDevice( std::vector<const char*>& extensions ) -> UniqueDevice
  {
    Helper::checkDeviceExtensionSupport( extensions );

    std::vector<DeviceQueueCreateInfo> queueCreateInfos;

    const float queuePriority = 1.0F;

    std::vector<uint32_t> queueFamilyIndices = { RAYEX_NAMESPACE::components::graphicsFamilyIndex, RAYEX_NAMESPACE::components::transferFamilyIndex };

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

    PhysicalDeviceAccelerationStructureFeaturesKHR asFeatures;
    asFeatures.accelerationStructure = VK_TRUE;

    // @todo Check for these features when evaluating physical devices.
    PhysicalDeviceRayTracingPipelineFeaturesKHR rtPipelineFeatures;
    rtPipelineFeatures.rayTracingPipeline                  = VK_TRUE;
    rtPipelineFeatures.rayTracingPipelineTraceRaysIndirect = VK_TRUE;
    rtPipelineFeatures.rayTraversalPrimitiveCulling        = VK_TRUE;
    rtPipelineFeatures.pNext                               = &asFeatures;

    PhysicalDeviceDescriptorIndexingFeatures indexingFeatures;
    indexingFeatures.runtimeDescriptorArray                        = VK_TRUE;
    indexingFeatures.shaderStorageBufferArrayNonUniformIndexing    = VK_TRUE;
    indexingFeatures.descriptorBindingVariableDescriptorCount      = VK_TRUE;
    indexingFeatures.descriptorBindingPartiallyBound               = VK_TRUE;
    indexingFeatures.descriptorBindingStorageBufferUpdateAfterBind = VK_TRUE;
    indexingFeatures.descriptorBindingUpdateUnusedWhilePending     = VK_TRUE;
    indexingFeatures.descriptorBindingSampledImageUpdateAfterBind  = VK_TRUE;
    indexingFeatures.shaderSampledImageArrayNonUniformIndexing     = VK_TRUE;
    indexingFeatures.pNext                                         = &rtPipelineFeatures;

    PhysicalDeviceRobustness2FeaturesEXT robustness2FeaturesEXT;
    robustness2FeaturesEXT.nullDescriptor = VK_TRUE;
    robustness2FeaturesEXT.pNext          = &indexingFeatures;

    PhysicalDeviceRayQueryFeaturesKHR rayQueryFeatures;
    rayQueryFeatures.rayQuery = VK_TRUE;
    rayQueryFeatures.pNext    = &robustness2FeaturesEXT;

    PhysicalDeviceBufferDeviceAddressFeatures bufferDeviceAddressFeatures;
    bufferDeviceAddressFeatures.bufferDeviceAddress = VK_TRUE;
    bufferDeviceAddressFeatures.pNext               = &rayQueryFeatures;

    PhysicalDeviceFeatures deviceFeatures;
    deviceFeatures.samplerAnisotropy = VK_TRUE;
    deviceFeatures.shaderInt64       = VK_TRUE;

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

    UniqueDevice device                 = RAYEX_NAMESPACE::components::physicalDevice.createDeviceUnique( createInfo );
    RAYEX_NAMESPACE::components::device = device.get( );
    RX_ASSERT( device.get( ), "Failed to create logical device." );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( device.get( ) );

    return std::move( device );
  }

  auto initInstance( const std::vector<const char*>& layers, std::vector<const char*>& extensions ) -> UniqueInstance
  {
    UniqueInstance instance;

    DynamicLoader dl;
    auto vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( vkGetInstanceProcAddr );

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

    instance                              = createInstanceUnique( createInfo );
    RAYEX_NAMESPACE::components::instance = instance.get( );
    RX_ASSERT( instance.get( ), "Failed to create instance." );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( instance.get( ) );

    return std::move( instance );
  }

  /*
  auto initGraphicsPipelinesUnique( const std::vector<GraphicsPipelineCreateInfo>& createInfos ) -> std::vector<UniquePipeline>
  {
    vk::Result result;
    std::vector<vk::Pipeline> temp;
    std::tie( result, temp ) = RAYEX_NAMESPACE::components::device.createGraphicsPipelines( nullptr, createInfos );

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
    std::tie( result, temp ) = RAYEX_NAMESPACE::components::device.createRayTracingPipelinesKHR( nullptr, createInfos );

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
