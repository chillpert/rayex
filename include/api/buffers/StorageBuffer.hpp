#ifndef STORAGE_BUFFER_HPP
#define STORAGE_BUFFER_HPP

#include "api/buffers/Buffer.hpp"
#include "base/Lights.hpp"

namespace RAYEX_NAMESPACE
{
  /// A wrapper for RAYEX_NAMESPACE::DirectionalLight matching the buffer alignment requirements.
  /// @ingroup API
  struct DirectionalLightSSBO
  {
    glm::vec4 ambient   = glm::vec4( 1.0F ); ///< Stores ambient color (vec3) and ambient intensity (float) in a vec4.
    glm::vec4 diffuse   = glm::vec4( 1.0F ); ///< Stores diffuse color (vec3) and diffuse intensity (float) in a vec4.
    glm::vec4 specular  = glm::vec4( 1.0F ); ///< Stores specular color (vec3) and specular intensity (float) in a vec4.
    glm::vec4 direction = glm::vec4( 1.0F ); ///< Stores the direction (vec3).
  };

  /// A wrapper for RAYEX_NAMESPACE::PointLight matching the buffer alignment requirements.
  /// @ingroup API
  struct PointLightSSBO
  {
    glm::vec4 ambient  = glm::vec4( 1.0F ); ///< Stores ambient color (vec3) and ambient intensity (float) in a vec4.
    glm::vec4 diffuse  = glm::vec4( 1.0F ); ///< Stores diffuse color (vec3) and diffuse intensity (float) in a vec4.
    glm::vec4 specular = glm::vec4( 1.0F ); ///< Stores specular color (vec3) and specular intensity (float) in a vec4.
    glm::vec4 position = glm::vec4( 1.0F ); ///< Stores the position (vec3).
  };

  /// A wrapper for RAYEX_NAMESPACE::MeshSSBO matching the buffer alignment requirements.
  /// @ingroup API
  struct MeshSSBO
  {
    glm::vec4 ambient  = glm::vec4( 1.0F, 1.0F, 1.0F, -1.0F ); // vec3 ambient  + vec1 texture index
    glm::vec4 diffuse  = glm::vec4( 0.2F, 1.0F, 1.0F, -1.0F ); // vec3 diffuse  + vec1 texture index
    glm::vec4 specular = glm::vec4( 1.0F, 1.0F, 1.0F, -1.0F ); // vec3 specular + vec1 texture index

    glm::vec4 padding0 = glm::vec4( 1.0F ); ///< Buffer padding (ignore).
    glm::vec4 padding1 = glm::vec4( 1.0F ); ///< Buffer padding (ignore).
    glm::vec4 padding2 = glm::vec4( 1.0F ); ///< Buffer padding (ignore).

    uint32_t indexOffset = 0; ///< Refers to the offset of this mesh inside a Geometry::indices container.

    uint32_t padding3 = 0; ///< Buffer padding (ignore).
    uint32_t padding4 = 0; ///< Buffer padding (ignore).
    uint32_t padding5 = 0; ///< Buffer padding (ignore).
  };

  /// A wrapper for RAYEX_NAMESPACE::GeometryInstanceSSBO matching the buffer alignment requirements.
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

  /// A shader storage buffer wrapper class.
  /// @ingroup API
  /// @todo documentation
  template <class T>
  class StorageBuffer
  {
  public:
    auto getDescriptorInfos( ) const -> const std::vector<vk::DescriptorBufferInfo>& { return _bufferInfos; }

    void init( const std::vector<T>& data, size_t copies = 1 )
    {
      _maxSize = sizeof( data[0] ) * data.size( );

      _stagingBuffers.resize( copies );
      _storageBuffers.resize( copies );
      _bufferInfos.resize( copies );
      _fences.resize( copies );

      for ( size_t i = 0; i < copies; ++i )
      {
        _stagingBuffers[i].init( _maxSize,                                                                               // size
                                 vk::BufferUsageFlagBits::eTransferSrc,                                                  // usage
                                 { components::transferFamilyIndex },                                                    // queueFamilyIndices
                                 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent ); // memoryPropertyFlags

        _storageBuffers[i].init( _maxSize,                                                                        // size
                                 vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eStorageBuffer, // usage
                                 { components::transferFamilyIndex },                                             // queueFamilyIndices
                                 vk::MemoryPropertyFlagBits::eDeviceLocal );                                      // memoryPropertyFlags

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
    /// @param data The data to upload.
    void upload( const std::vector<T>& data, std::optional<uint32_t> index = { } )
    {
      RX_ASSERT( _maxSize >= sizeof( data[0] ) * data.size( ), "Exceeded maximum storage buffer size." );

      if ( !index.has_value( ) )
      {
        for ( size_t i = 0; i < _storageBuffers.size( ); ++i )
        {
          //components::device.waitForFences( 1, &_fences[i].get( ), VK_TRUE, UINT64_MAX );
          //components::device.resetFences( 1, &_fences[i].get( ) );

          _stagingBuffers[i].fill<T>( data );
          _stagingBuffers[i].copyToBuffer( _storageBuffers[i].get( ) ); //, _fences[i].get( ) );
        }
      }
      else
      {
        //components::device.waitForFences( 1, &_fences[index.value( )].get( ), VK_TRUE, UINT64_MAX );
        //components::device.resetFences( 1, &_fences[index.value( )].get( ) );

        _stagingBuffers[index.value( )].fill<T>( data );
        _stagingBuffers[index.value( )].copyToBuffer( _storageBuffers[index.value( )].get( ) ); //, _fences[index.value( )].get( ) );
      }
    }

  private:
    std::vector<Buffer> _stagingBuffers;
    std::vector<Buffer> _storageBuffers;

    std::vector<vk::DescriptorBufferInfo> _bufferInfos;
    std::vector<vk::UniqueFence> _fences;

    vk::DeviceSize _maxSize = 0;
  };
} // namespace RAYEX_NAMESPACE

#endif // STORAGE_BUFFER_HPP
