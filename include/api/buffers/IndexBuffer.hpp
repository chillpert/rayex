#pragma once

#include "api/buffers/Buffer.hpp"

namespace RAYEX_NAMESPACE
{
  /// A specialised buffer for index data.
  /// @note If not templatized then uint32_t will be used as the data's type.
  /// @ingroup API
  class IndexBuffer : public Buffer
  {
  public:
    /// @return Returns the amount of indices in the buffer.
    auto getCount( ) const -> uint32_t { return _count; }

    /// Creates the buffer, allocates memory for it and fills it with the provided data.
    ///
    /// To optimize the procedure a temporary staging buffer will be created.
    /// @param indices The index data.
    void init( const std::vector<uint32_t>& indices )
    {
      _count = static_cast<uint32_t>( indices.size( ) );

      vk::DeviceSize size = sizeof( indices[0] ) * indices.size( );
      vk::MemoryAllocateFlagsInfo allocateFlags( vk::MemoryAllocateFlagBitsKHR::eDeviceAddress );

      // Set up the staging buffer.
      Buffer stagingBuffer( size,
                            vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eShaderDeviceAddress,
                            { components::transferFamilyIndex },
                            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                            &allocateFlags );

      stagingBuffer.fill<uint32_t>( indices );

      // Set up the actual index buffer.
      Buffer::init( size,
                    vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eShaderDeviceAddress,
                    { components::transferFamilyIndex },
                    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                    &allocateFlags );

      // Copy staging buffer to the actual index buffer.
      stagingBuffer.copyToBuffer( _buffer.get( ) );
    }

  private:
    uint32_t _count = 0; ///< The amount of indices in the buffer.
  };
} // namespace RAYEX_NAMESPACE
