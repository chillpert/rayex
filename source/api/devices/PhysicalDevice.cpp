#include "api/devices/PhysicalDevice.hpp"
#include "api/Components.hpp"
#include "api/Queues.hpp"

namespace RENDERER_NAMESPACE
{
  void PhysicalDevice::init( )
  {
    auto physicalDevices = g_instance.enumeratePhysicalDevices( );

    std::vector<std::pair<unsigned int, std::string>> results;

    unsigned int score = 0;
    for ( const auto& it : physicalDevices )
    {
      auto temp = evaluate( it );
      results.push_back( temp );

      if ( temp.first > score )
      {
        m_physicalDevice = it;
        score = temp.first;
      }
    }

    const std::string separator = "===================================================================";

    if ( !m_physicalDevice )
      RX_FATAL( "No suitable device was found" );

    std::cout << "Physical device report: " << "\n" << separator << "\n" << "Device name" << "\t\t\t" << "Score" << std::endl << separator << "\n";

    for ( const auto& result : results )
      std::cout << std::left << std::setw( 32 ) << std::setfill( ' ' ) << result.second << std::left << std::setw( 32 ) << std::setfill( ' ' ) << result.first << std::endl;

    // Print information about the GPU that was selected.
    auto properties = m_physicalDevice.getProperties( );
    RX_SUCCESS( "Selected GPU: ", properties.deviceName );

    g_physicalDeviceLimits = properties.limits;
    g_physicalDevice = m_physicalDevice;
  }

  std::pair<unsigned int, std::string> PhysicalDevice::evaluate( vk::PhysicalDevice physicalDevice ) const
  {
    unsigned int score = 0u;

    // Check the device's features and properties.
    auto props = physicalDevice.getProperties( );
    auto feats = physicalDevice.getFeatures( );
    auto rayTracingFeats = getRayTracingFeatures( physicalDevice );

    std::string deviceName = props.deviceName;

    if ( rayTracingFeats.rayTracing == VK_FALSE || rayTracingFeats.rayQuery == VK_FALSE )
      return { 0u, deviceName };

    if ( props.deviceType == vk::PhysicalDeviceType::eDiscreteGpu )
      score += 100u;
    else
      return { 0u, deviceName };

  #ifdef VK_API_VERSION_1_1
    if ( props.apiVersion >= VK_API_VERSION_1_1 )
      score += 10u;
  #endif

  #ifdef VK_API_VERSION_1_2
    if ( props.apiVersion >= VK_API_VERSION_1_2 )
      score += 10u;
  #endif

    if ( deviceName.find( "RTX" ) != std::string::npos )
      score += 100u;

    if ( Queues::isComplete( physicalDevice ) )
      score += 100u;
    else
      return { 0u, deviceName };

    if ( Queues::hasDedicatedTransferQueueFamily( physicalDevice ) )
      score += 25;

    // TODO: add more hardware specific evaulation (those that are benefitial for path tracing)
    // https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#features

    return { score, deviceName };
  }

  const vk::PhysicalDeviceRayTracingFeaturesKHR getRayTracingFeatures( vk::PhysicalDevice physicalDevice )
  {
    vk::PhysicalDeviceRayTracingFeaturesKHR rayTracingFeatures;

    vk::PhysicalDeviceFeatures2 features2;
    features2.setPNext( &rayTracingFeatures );

    physicalDevice.getFeatures2( &features2 );

    return rayTracingFeatures;
  }
}