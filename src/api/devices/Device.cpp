#include "Device.hpp"
#include "Components.hpp"

namespace rx
{
  Device::Device( std::vector<const char*>& extensions, bool initialize )
  {
    if ( initialize )
      init( extensions );
  }

  void Device::init( std::vector<const char*>& extensions )
  {
    extensions.push_back( VK_KHR_SWAPCHAIN_EXTENSION_NAME );
    checkExtensionSupport( extensions );

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

    const float queuePriority = 1.0f;

    std::vector<uint32_t> queueFamilyIndices =
    {
      g_graphicsFamilyIndex,
      g_transferFamilyIndex
    };

    // Note: This process takes about 1.5 seconds on a Ryzen 5 3600x + RTX 2070. Maybe it's not worth creating all available queues.
    uint32_t index = 0;
    for ( const auto& queueFamilyIndex : queueFamilyIndices )
    {
      vk::DeviceQueueCreateInfo queueCreateInfo( { },              // flags 
                                                 queueFamilyIndex, // queueFamilyIndex
                                                 1,                // queueCount
                                                 &queuePriority ); // pQueuePriorties

      queueCreateInfos.push_back( queueCreateInfo );

      ++index;
    }

    vk::PhysicalDeviceRayTracingFeaturesKHR rayTracingFeatures;
    rayTracingFeatures.rayTracing = VK_TRUE;
    rayTracingFeatures.rayQuery = VK_TRUE;

    vk::PhysicalDeviceBufferDeviceAddressFeatures bufferDeviceAddressFeatures;
    bufferDeviceAddressFeatures.bufferDeviceAddress = VK_TRUE;
    bufferDeviceAddressFeatures.pNext = &rayTracingFeatures;

    vk::PhysicalDeviceFeatures deviceFeatures;
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    vk::PhysicalDeviceFeatures2 deviceFeatures2{ deviceFeatures };
    deviceFeatures2.pNext = &bufferDeviceAddressFeatures;

    vk::DeviceCreateInfo createInfo( { },                                                 // flags
                                     static_cast< uint32_t >( queueCreateInfos.size() ),  // queueCreateInfoCount
                                     queueCreateInfos.data(),                             // pQueueCreateInfos
                                     0,                                                   // enabledLayerCount
                                     nullptr,                                             // ppEnabledLayerNames
                                     static_cast< uint32_t >( extensions.size() ),        // enabledExtensionCount
                                     extensions.data(),                                   // ppEnabledExtensionNames
                                     nullptr );                                           // pEnabledFeatures - must be NULL because the VkDeviceCreateInfo::pNext chain includes VkPhysicalDeviceFeatures2.

    createInfo.pNext = &deviceFeatures2;

    m_device = g_physicalDevice.createDeviceUnique( createInfo );
    g_device = m_device.get();

    if ( !m_device )
      RX_ERROR( "Failed to create logical device." );

    VULKAN_HPP_DEFAULT_DISPATCHER.init( m_device.get( ) );
  }

  void Device::checkExtensionSupport( const std::vector<const char*>& extensions ) const
  {
    // Stores the name of the extension and a bool that tells if they were found or not.
    std::map<const char*, bool> requiredExtensions;

    for ( const auto& extension : extensions )
      requiredExtensions.emplace( extension, false );

    std::vector<vk::ExtensionProperties> physicalDeviceExtensions = g_physicalDevice.enumerateDeviceExtensionProperties();

    // Iterates over all enumerated physical device extensions to see if they are available.
    for ( const auto& physicalDeviceExtension : physicalDeviceExtensions )
    {
      for ( auto& requiredphysicalDeviceExtension : requiredExtensions )
      {
        if ( strcmp( physicalDeviceExtension.extensionName, requiredphysicalDeviceExtension.first ) == 0 )
          requiredphysicalDeviceExtension.second = true;
      }
    }

    // Give feedback on the previous operations.
    for ( const auto& requiredphysicalDeviceExtension : requiredExtensions )
    {
      if ( !requiredphysicalDeviceExtension.second )
        RX_ERROR( "Missing physical device extension: " + requiredphysicalDeviceExtension.first + ". Have you tried installing the NVIDIA Beta drivers?" );
      else
        RX_LOG( "Added device extension: " << requiredphysicalDeviceExtension.first );
    }
  }
}