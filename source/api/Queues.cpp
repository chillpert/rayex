#include "api/Queues.hpp"
#include "api/misc/Components.hpp"

namespace RENDERER_NAMESPACE
{
  void Queues::init( )
  {
    auto queueFamilyProperties = g_physicalDevice.getQueueFamilyProperties( );
    std::vector<uint32_t> queueFamilies( queueFamilyProperties.size( ) );

    bool dedicatedTransferQueueFamily = hasDedicatedTransferQueueFamily( g_physicalDevice );

    for ( uint32_t index = 0; index < static_cast< uint32_t >( queueFamilies.size( ) ); ++index )
    {
      if ( queueFamilyProperties[index].queueFlags & vk::QueueFlagBits::eGraphics && !this->graphicsFamilyIndex.has_value( ) )
      {
        if ( g_physicalDevice.getSurfaceSupportKHR( index, g_surface ) )
          this->graphicsFamilyIndex = index;
      }

      if ( dedicatedTransferQueueFamily )
      {
        if ( !( queueFamilyProperties[index].queueFlags & vk::QueueFlagBits::eGraphics ) && !this->transferFamilyIndex.has_value( ) )
          this->transferFamilyIndex = index;
      }
      else
      {
        if ( queueFamilyProperties[index].queueFlags & vk::QueueFlagBits::eTransfer && !this->transferFamilyIndex.has_value( ) )
          this->transferFamilyIndex = index;
      }
    }

    g_graphicsFamilyIndex = this->graphicsFamilyIndex.value( );
    g_transferFamilyIndex = this->transferFamilyIndex.value( );

    if ( !this->graphicsFamilyIndex.has_value( ) || !this->transferFamilyIndex.has_value( ) )
      RX_ERROR( "Failed to retrieve queue family indices." );
  }

  void Queues::retrieveHandles( )
  {
    if ( !this->graphicsFamilyIndex.has_value( ) || !this->transferFamilyIndex.has_value( ) )
      RX_ERROR( "Failed to retrieve queue handles." );

    g_device.getQueue( this->graphicsFamilyIndex.value( ), 0, &g_graphicsQueue );
    g_device.getQueue( this->transferFamilyIndex.value( ), 0, &g_transferQueue );
  }

  bool Queues::isComplete( vk::PhysicalDevice physicalDevice )
  {
    auto queueFamilies = physicalDevice.getQueueFamilyProperties( );
    uint32_t queueFamilyIndicesCount = static_cast< uint32_t >( queueFamilies.size( ) );

    // Get all possible queue family indices with transfer support.
    std::vector<uint32_t> graphicsQueueFamilyIndices;
    std::vector<uint32_t> transferQueueFamilyIndices;
    std::vector<uint32_t> computeQueueFamilyIndices;

    for ( uint32_t index = 0; index < queueFamilyIndicesCount; ++index )
    {
      // Make sure the current queue family index contains at least one queue.
      if ( queueFamilies[index].queueCount == 0 )
        continue;

      if ( queueFamilies[index].queueFlags & vk::QueueFlagBits::eGraphics )
      {
        if ( physicalDevice.getSurfaceSupportKHR( index, g_surface ) )
          graphicsQueueFamilyIndices.push_back( index );
      }

      if ( queueFamilies[index].queueFlags & vk::QueueFlagBits::eTransfer )
        transferQueueFamilyIndices.push_back( index );

      if ( queueFamilies[index].queueFlags & vk::QueueFlagBits::eCompute )
        computeQueueFamilyIndices.push_back( index );
    }

    if ( graphicsQueueFamilyIndices.size( ) == 0 || computeQueueFamilyIndices.size( ) == 0 || transferQueueFamilyIndices.size( ) == 0 )
      return false;

    return true;
  }

  bool Queues::hasDedicatedTransferQueueFamily( vk::PhysicalDevice physicalDevice )
  {
    auto queueFamilyProperties = physicalDevice.getQueueFamilyProperties( );

    for ( uint32_t index = 0; index < static_cast< uint32_t >( queueFamilyProperties.size( ) ); ++index )
    {
      if ( !( queueFamilyProperties[index].queueFlags & vk::QueueFlagBits::eGraphics ) )
        return true;
    }

    return false;
  }
}