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
  /// @todo make this class a template and not the functions inside
  class StorageBuffer : public Buffer
  {
  public:
    StorageBuffer( ) = default;

    template <typename T>
    void init( const std::vector<T>& data, vk::DeviceSize offset = 0 )
    {
      vk::DeviceSize size = sizeof( data[0] ) * data.size( );

      // Set up the staging buffer.
      _stagingBuffer.init( size,                                                                                   // size
                           vk::BufferUsageFlagBits::eTransferSrc,                                                  // usage
                           { components::transferFamilyIndex },                                                    // queueFamilyIndices
                           vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent ); // memoryPropertyFlags

      Buffer::init( size,                                                                            // size
                    vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eStorageBuffer, // usage
                    { components::transferFamilyIndex },                                             // queueFamilyIndices
                    vk::MemoryPropertyFlagBits::eDeviceLocal );                                      // memoryPropertyFlags

      upload<T>( data );
    }

    /// Uploads data to the buffer.
    ///
    /// First, the data is being copied to the staging buffer which is visible to the host.
    /// Finally, the staging buffer is copied to the actual buffer on the device.
    /// @param data The data to upload.
    template <typename T>
    void upload( const std::vector<T>& data )
    {
      _stagingBuffer.fill<T>( data.data( ) );
      _stagingBuffer.copyToBuffer( _buffer.get( ) );
    }

    Buffer _stagingBuffer;
  };
} // namespace RAYEX_NAMESPACE

#endif // STORAGE_BUFFER_HPP
