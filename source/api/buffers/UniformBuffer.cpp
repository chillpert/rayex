#include "api/buffers/UniformBuffer.hpp"
#include "api/misc/Components.hpp"

namespace RAYEXEC_NAMESPACE
{
  const std::vector<vk::Buffer> UniformBuffer::getRaw( ) const
  {
    std::vector<vk::Buffer> res( this->buffers.size( ), nullptr );

    for ( size_t i = 0; i < this->buffers.size( ); ++i )
      res[i] = this->buffers[i].get( );

    /*
    std::vector<vk::Buffer> res;
    res.reserve( this->buffers.size( ) );

    for ( const auto& buffer : this->buffers )
      res.push_back( buffer.get( ) );
    */

    return res;
  }

  std::vector<vk::DescriptorBufferInfo> UniformBuffer::getDescriptorInfos( vk::DeviceSize size )
  {
    RX_ASSERT( g_swapchainImageCount > 0, "Failed to get descriptor infos of uniform buffer because there are no swapchain images." );

    std::vector<vk::DescriptorBufferInfo> bufferInfos( g_swapchainImageCount );
    for ( size_t i = 0; i < this->buffers.size( ); ++i )
    {
      bufferInfos[i] = { this->buffers[i].get( ), 0, sizeof( size ) };
    }

    return bufferInfos;
  }
}