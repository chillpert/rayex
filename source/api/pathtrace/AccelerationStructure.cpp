#include "api/pathtrace/AccelerationStructure.hpp"

#include "api/Components.hpp"

namespace RAYEX_NAMESPACE
{
  void AccelerationStructure::destroy( )
  {
    if ( as )
    {
      vkCore::global::device.destroyAccelerationStructureKHR( as );
    }

    if ( buffer )
    {
      vkCore::global::device.destroyBuffer( buffer );
    }

    if ( memory )
    {
      vkCore::global::device.freeMemory( memory );
    }
  }

  auto initAccelerationStructure( vk::AccelerationStructureCreateInfoKHR& asCreateInfo ) -> AccelerationStructure
  {
    RAYEX_NAMESPACE::AccelerationStructure resultAs;

    vk::MemoryAllocateFlagsInfo allocateFlags( vk::MemoryAllocateFlagBitsKHR::eDeviceAddress );

    vk::BufferCreateInfo createInfo( { },                                                                                                       // flags
                                     asCreateInfo.size,                                                                                         // size
                                     vk::BufferUsageFlagBits::eAccelerationStructureStorageKHR | vk::BufferUsageFlagBits::eShaderDeviceAddress, // usage
                                     vk::SharingMode::eExclusive,                                                                               // sharingMode
                                     1,                                                                                                         // queueFamilyIndexCount
                                     &vkCore::global::graphicsFamilyIndex );                                                                    // pQueueFamilyIndices

    resultAs.buffer = vkCore::global::device.createBuffer( createInfo );
    RX_ASSERT( resultAs.buffer, "Failed to create buffer." );

    resultAs.memory = vkCore::allocateMemory( resultAs.buffer,
                                              vk::MemoryPropertyFlagBits::eDeviceLocal | vk::MemoryPropertyFlagBits::eHostCoherent,
                                              &allocateFlags );

    vkCore::global::device.bindBufferMemory( resultAs.buffer, resultAs.memory, 0 );

    asCreateInfo.buffer = resultAs.buffer;

    resultAs.as = vkCore::global::device.createAccelerationStructureKHR( asCreateInfo, nullptr );

    return resultAs;
  }
} // namespace RAYEX_NAMESPACE
