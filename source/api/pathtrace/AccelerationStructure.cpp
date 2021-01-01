#include "api/pathtrace/AccelerationStructure.hpp"

#include "api/Components.hpp"

namespace RAYEX_NAMESPACE
{
  void AccelerationStructure::destroy( )
  {
    if ( as )
    {
      components::device.destroyAccelerationStructureKHR( as );
    }

    if ( buffer )
    {
      components::device.destroyBuffer( buffer );
    }

    if ( memory )
    {
      components::device.freeMemory( memory );
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
                                     &rx::components::graphicsFamilyIndex );                                                                    // pQueueFamilyIndices

    resultAs.buffer = rx::components::device.createBuffer( createInfo );
    RX_ASSERT( resultAs.buffer, "Failed to create buffer." );

    resultAs.memory = vkCore::allocateMemory( resultAs.buffer,
                                              vk::MemoryPropertyFlagBits::eDeviceLocal | vk::MemoryPropertyFlagBits::eHostCoherent,
                                              &allocateFlags );

    RAYEX_NAMESPACE::components::device.bindBufferMemory( resultAs.buffer, resultAs.memory, 0 );

    asCreateInfo.buffer = resultAs.buffer;

    resultAs.as = RAYEX_NAMESPACE::components::device.createAccelerationStructureKHR( asCreateInfo, nullptr );

    return resultAs;
  }
} // namespace RAYEX_NAMESPACE
