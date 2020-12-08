#pragma once

#include "api/buffers/Buffer.hpp"
#include "api/utility/Initializers.hpp"

namespace RAYEX_NAMESPACE
{
  /// A wrapper for MeshSSBO matching the buffer alignment requirements.
  /// @ingroup API
  struct MeshSSBO
  {
    glm::vec4 ambient  = glm::vec4( 1.0F, 1.0F, 1.0F, -1.0F ); // vec3 ambient  + vec1 texture index
    glm::vec4 diffuse  = glm::vec4( 0.2F, 1.0F, 1.0F, -1.0F ); // vec3 diffuse  + vec1 texture index
    glm::vec4 specular = glm::vec4( 1.0F, 1.0F, 1.0F, -1.0F ); // vec3 specular + vec1 texture index

    glm::vec4 emission  = glm::vec4( 1.0F );
    glm::vec4 emittance = glm::vec4( 1.0F );
    glm::vec4 padding0  = glm::vec4( 1.0F ); ///< Buffer padding (ignore).

    uint32_t indexOffset = 0; ///< Refers to the offset of this mesh inside a Geometry::indices container.

    uint32_t padding1 = 0; ///< Buffer padding (ignore).
    uint32_t padding2 = 0; ///< Buffer padding (ignore).
    uint32_t padding3 = 0; ///< Buffer padding (ignore).
  };

  /// A wrapper for GeometryInstanceSSBO matching the buffer alignment requirements.
  /// @ingroup API
  struct GeometryInstanceSSBO
  {
    glm::mat4 transform    = glm::mat4( 1.0F ); ///< The instance's world transform matrix.
    glm::mat4 transformIT  = glm::mat4( 1.0F ); ///< The inverse transpose of transform.
    uint32_t geometryIndex = 0;                 ///< Used to assign this instance a model.

    float padding0 = 0.0F; ///< Buffer padding (ignore).
    float padding1 = 0.0F; ///< Buffer padding (ignore).
    float padding2 = 0.0F; ///< Buffer padding (ignore).
  };

  /// A shader storage buffer specilization class.
  /// @ingroup API
  template <class T>
  class StorageBuffer
  {
  public:
    auto get( ) const -> const std::vector<Buffer>& { return _storageBuffers; }

    auto get( size_t index ) const -> const vk::Buffer { return _storageBuffers[index].get( ); }

    auto getCount( ) const -> uint32_t { return _count; }

    auto getDescriptorInfos( ) const -> const std::vector<vk::DescriptorBufferInfo>& { return _bufferInfos; }

    /// Creates a storage buffer and n copies.
    /// @param data The data to fill the storage buffer(s) with.
    /// @param copies The amount of copies to make.
    /// @param deviceAddressVisible If true, the buffer will be device visible.
    void init( const std::vector<T>& data, size_t copies = 1, bool deviceAddressVisible = false )
    {
      _count = static_cast<uint32_t>( data.size( ) );

      _maxSize = sizeof( data[0] ) * data.size( );

      _stagingBuffers.resize( copies );
      _storageBuffers.resize( copies );
      _bufferInfos.resize( copies );
      _fences.resize( copies );

      vk::MemoryAllocateFlagsInfo* allocateFlags = nullptr;
      vk::MemoryAllocateFlagsInfo temp( vk::MemoryAllocateFlagBitsKHR::eDeviceAddress );

      if ( deviceAddressVisible )
      {
        allocateFlags = &temp;
      }

      for ( size_t i = 0; i < copies; ++i )
      {
        _stagingBuffers[i].init( _maxSize,                                                                             // size
                                 vk::BufferUsageFlagBits::eTransferSrc,                                                // usage
                                 { components::transferFamilyIndex },                                                  // queueFamilyIndices
                                 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, // memoryPropertyFlags
                                 allocateFlags );

        vk::BufferUsageFlags bufferUsageFlags = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eStorageBuffer;
        if ( deviceAddressVisible )
        {
          bufferUsageFlags |= vk::BufferUsageFlagBits::eShaderDeviceAddress;
        }

        _storageBuffers[i].init( _maxSize,                                 // size
                                 bufferUsageFlags,                         // usage
                                 { components::transferFamilyIndex },      // queueFamilyIndices
                                 vk::MemoryPropertyFlagBits::eDeviceLocal, // memoryPropertyFlags
                                 allocateFlags );

        _bufferInfos[i].buffer = _storageBuffers[i].get( );
        _bufferInfos[i].offset = 0;
        _bufferInfos[i].range  = VK_WHOLE_SIZE;

        _fences[i] = vk::Initializer::initFenceUnique( vk::FenceCreateFlagBits::eSignaled );
      }

      upload( data );
    }

    /// Uploads data to the buffer.
    ///
    /// First, the data is being copied to the staging buffer which is visible to the host.
    /// Finally, the staging buffer is copied to the actual buffer on the device.
    /// @param index Optionally used in case the data should only be uploaded to a specific buffer.
    void upload( const std::vector<T>& data, std::optional<uint32_t> index = { } )
    {
      RX_ASSERT( _maxSize >= sizeof( data[0] ) * data.size( ), "Exceeded maximum storage buffer size." );

      if ( !index.has_value( ) )
      {
        for ( size_t i = 0; i < _storageBuffers.size( ); ++i )
        {
          _stagingBuffers[i].fill<T>( data );
          _stagingBuffers[i].copyToBuffer( _storageBuffers[i].get( ) );
        }
      }
      else
      {
        _stagingBuffers[index.value( )].fill<T>( data );
        _stagingBuffers[index.value( )].copyToBuffer( _storageBuffers[index.value( )].get( ) );
      }
    }

  private:
    std::vector<Buffer> _stagingBuffers; ///< Holds the staging buffer and all its copies.
    std::vector<Buffer> _storageBuffers; ///< Holds the storage buffer and all its copies.

    std::vector<vk::DescriptorBufferInfo> _bufferInfos;
    std::vector<vk::UniqueFence> _fences;

    vk::DeviceSize _maxSize = 0;
    uint32_t _count         = 0;
  };
} // namespace RAYEX_NAMESPACE
