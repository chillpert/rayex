#ifndef STORAGE_BUFFER_HPP
#define STORAGE_BUFFER_HPP

#include "api/buffers/Buffer.hpp"
#include "base/Lights.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// A wrapper for RAYEXEC_NAMESPACE::Light matching the buffer alignment requirements.
  struct LightSSBO
  {
    glm::vec4 ambient  = glm::vec4( 1.0F ); ///< Stores ambient color (vec3) and ambient intensity (float) in a vec4.
    glm::vec4 diffuse  = glm::vec4( 1.0F ); ///< Stores diffuse color (vec3) and diffuse intensity (float) in a vec4.
    glm::vec4 specular = glm::vec4( 1.0F ); ///< Stores specular color (vec3) and specular intensity (float) in a vec4.
  };

  /// A wrapper for RAYEXEC_NAMESPACE::DirectionalLight matching the buffer alignment requirements.
  struct DirectionalLightSSBO : public LightSSBO
  {
    glm::vec4 direction = glm::vec4( 1.0F ); ///< Stores the direction (vec3).
  };

  struct PointLightSSBO : public LightSSBO
  {
    glm::vec4 position = glm::vec4( 1.0F ); ///< Stores the position (vec3).
  };

  class StorageBuffer : public Buffer
  {
  public:
    StorageBuffer( ) = default;

    template <typename T>
    void init( const std::vector<T>& data )
    {
      vk::DeviceSize size = sizeof( data[0] ) * data.size( );

      // Set up the staging buffer.
      Buffer stagingBuffer( size,                                                                                   // size
                            vk::BufferUsageFlagBits::eTransferSrc,                                                  // usage
                            { g_transferFamilyIndex },                                                              // queueFamilyIndices
                            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent ); // memoryPropertyFlags

      stagingBuffer.fill<T>( data.data( ) );

      Buffer::init( size,                                                                                  // size
                    vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eStorageBuffer,       // usage
                    { g_transferFamilyIndex },                                                             // queueFamilyIndices
                    vk::MemoryPropertyFlagBits::eDeviceLocal | vk::MemoryPropertyFlagBits::eHostVisible ); // memoryPropertyFlags                                                                                                                // pNext of memory

      // Copy staging buffer to the actual index buffer.
      stagingBuffer.copyToBuffer( this->buffer.get( ) );
    }
  };
} // namespace RAYEXEC_NAMESPACE

#endif // STORAGE_BUFFER_HPP
