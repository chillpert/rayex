#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

#include "api/buffers/Buffer.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// A specialised buffer for index data.
  /// @note If not templatized then uint32_t will be used as the data's type.
  /// @ingroup API
  class IndexBuffer : public Buffer
  {
  public:
    /// @return Returns the amount of indices in the buffer.
    [[nodiscard]] auto getCount( ) const -> uint32_t { return count; }

    /// Creates the buffer, allocates memory for it and fills it with the provided data.
    ///
    /// To optimize the procedure a temporary staging buffer will be created.
    /// @param indices The index data.
    void init( const std::vector<uint32_t>& indices )
    {
      count = static_cast<uint32_t>( indices.size( ) );

      vk::DeviceSize size = sizeof( indices[0] ) * indices.size( );
      vk::MemoryAllocateFlagsInfo allocateFlags( vk::MemoryAllocateFlagBitsKHR::eDeviceAddress );

      // Set up the staging buffer.
      Buffer stagingBuffer( size,
                            vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eShaderDeviceAddress,
                            { g_transferFamilyIndex },
                            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                            &allocateFlags );

      stagingBuffer.fill<uint32_t>( indices.data( ) );

      // Set up the actual index buffer.
      Buffer::init( size,
                    vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eShaderDeviceAddress,
                    { g_transferFamilyIndex },
                    vk::MemoryPropertyFlagBits::eDeviceLocal,
                    &allocateFlags );

      // Copy staging buffer to the actual index buffer.
      stagingBuffer.copyToBuffer( buffer.get( ) );
    }

  private:
    uint32_t count; ///< The amount of indices in the buffer.
  };
} // namespace RAYEXEC_NAMESPACE

#endif // INDEX_BUFFER_HPP
