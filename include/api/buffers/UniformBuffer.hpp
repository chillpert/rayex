#ifndef UNIFORM_BUFFER_HPP
#define UNIFORM_BUFFER_HPP

#include "api/buffers/Buffer.hpp"
#include "base/Geometry.hpp"

namespace RAYEX_NAMESPACE
{
  /// A uniform buffer object for camera data.
  /// @todo Camera position could easily be a push constant, which would make this UBO much smaller in size.
  /// @ingroup API
  struct CameraUbo
  {
    glm::mat4 view              = glm::mat4( 1.0F );
    glm::mat4 projection        = glm::mat4( 1.0F );
    glm::mat4 viewInverse       = glm::mat4( 1.0F );
    glm::mat4 projectionInverse = glm::mat4( 1.0F );
    glm::vec4 position          = glm::vec4( 1.0F );

  private:
    glm::vec4 padding0 = glm::vec4( 1.0F );
    glm::vec4 padding1 = glm::vec4( 1.0F );
    glm::vec4 padding2 = glm::vec4( 1.0F );
  };

  /// A specialised buffer for uniforms.
  /// @ingroup API
  class UniformBuffer
  {
  public:
    UniformBuffer( ) = default;

    /// @param swapchainImagesCount The amount of images in the swapchain.
    /// @param initialize If true, the uniform buffer will be initialized right away without an additional call to init().
    template <typename T>
    UniformBuffer( size_t swapchainImagesCount, bool initialize = true )
    {
      if ( initialize )
        init<T>( swapchainImagesCount );
    }

    /// @return Returns the vector of uniform buffers.
    [[nodiscard]] auto get( ) const -> const std::vector<Buffer>& { return buffers; }

    /// @return Returns the vector of uniform buffers as raw Vulkan buffer objects.
    [[nodiscard]] RX_API auto getRaw( ) const -> const std::vector<vk::Buffer>;

    /// Creates the uniform buffer and allocates memory for it.
    ///
    /// The function will create as many uniform buffers as there are images in the swapchain.
    /// Additionally, it will create the descriptor buffer infos which can be later used to write to a descriptor set.
    /// @param swapchainImagesCount The amount of images in the swapchain.
    template <typename T>
    void init( )
    {
      size_t swapchainImageCount = static_cast<size_t>( g_swapchainImageCount );

      buffers.resize( swapchainImageCount );

      for ( Buffer& buffer : buffers )
      {
        buffer.init( sizeof( T ),
                     vk::BufferUsageFlagBits::eUniformBuffer,
                     { },
                     vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );
      }

      this->bufferInfos.resize( swapchainImageCount );
      for ( size_t i = 0; i < this->buffers.size( ); ++i )
        this->bufferInfos[i] = vk::DescriptorBufferInfo( this->buffers[i].get( ), 0, sizeof( T ) );
    }

    /// Used to fill an image's buffer.
    /// @param imageIndex The image's index in the swapchain.
    /// @param ubo The actual uniform buffer object holding the data.
    template <typename T>
    void upload( uint32_t imageIndex, T& ubo )
    {
      buffers[imageIndex].fill<T>( &ubo );
    }

    std::vector<vk::DescriptorBufferInfo> bufferInfos;

  private:
    std::vector<Buffer> buffers; ///< A vector of RAYEX_NAMESPACE::Buffers for the uniform buffers.
  };
} // namespace RAYEX_NAMESPACE

#endif // UNIFORM_BUFFER_HPP
