#include "api/Instance.hpp"
#include "api/misc/Components.hpp"

#define VULKAN_HPP_STORAGE_SHARED
#define VULKAN_HPP_STORAGE_SHARED_EXPORT
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace RENDERER_NAMESPACE
{
  Instance::Instance( const std::vector<const char*>& layers, std::vector<const char*>& extensions, bool initialize )
  {
    if ( initialize )
      init( layers, extensions );
  }

  void Instance::init( const std::vector<const char*>& layers, std::vector<const char*>& extensions )
  {
    vk::DynamicLoader dl;
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( vkGetInstanceProcAddr );

    // Retrieve all extensions needed by SDL2.
    std::vector<const char*> windowExtensions = g_window->getInstanceExtensions( );
    extensions.insert( extensions.end( ), windowExtensions.begin( ), windowExtensions.end( ) );

    // Check if all extensions and layers needed are available.
    checkLayersSupport( layers );
    checkExtensionsSupport( extensions );

    // Start creating the instance.
    vk::ApplicationInfo appInfo;
    appInfo.apiVersion = getApiVersion( );

    vk::InstanceCreateInfo createInfo( { },                                         // flags
                                       &appInfo,                                    // pApplicationInfo
                                       static_cast<uint32_t>( layers.size( ) ),     // enabledLayerCount
                                       layers.data( ),                              // ppEnabledLayerNames
                                       static_cast<uint32_t>( extensions.size( ) ), // enabledExtensionCount
                                       extensions.data( ) );                        // ppEnabledExtensionNames

    m_instance = vk::createInstanceUnique( createInfo );
    g_instance = m_instance.get( );

    if ( !m_instance )
      RX_FATAL( "Failed to create instance." );

    VULKAN_HPP_DEFAULT_DISPATCHER.init( m_instance.get( ) );
  }

  void Instance::checkLayersSupport( const std::vector<const char*>& layers )
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
        RX_ERROR( "Validation layer ", name, " is not available on this device" );

      RX_SUCCESS( "Added layer: ", name );
    }
  }

  void Instance::checkExtensionsSupport( const std::vector<const char*>& extensions )
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
        RX_ERROR( "Instance extensions ", name, " is not available on this device" );

      RX_SUCCESS( "Added extension: ", name );
    }
  }

  uint32_t Instance::getApiVersion( )
  {
    uint32_t apiVersion = vk::enumerateInstanceVersion( );

  #ifdef VK_API_VERSION_1_2
    if ( apiVersion >= VK_API_VERSION_1_2 )
    {
      RX_SUCCESS( "Found Vulkan SDK API version 1.2" );
      return VK_API_VERSION_1_2;
    }
  #endif

  #ifdef VK_API_VERSION_1_1
    if ( apiVersion >= VK_API_VERSION_1_1 )
    {
      RX_SUCCESS( "Found Vulkan SDK API version 1.1" );
      return VK_API_VERSION_1_1;
    }
  #endif

    RX_FATAL( "This application requires Vulkan SDK API Version 1.1 or higher." );

    return 0;
  }
}