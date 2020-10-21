#include "api/Components.hpp"
#include "api/buffers/Buffer.hpp"
#include "api/buffers/CommandBuffer.hpp"
#include "api/image/Texture.hpp"
#include "api/utility/Helpers.hpp"
#include "api/utility/Initializers.hpp"

namespace RAYEX_NAMESPACE
{
  void Image::init( const vk::ImageCreateInfo& createInfo )
  {
    _extent = createInfo.extent;
    _format = createInfo.format;
    _layout = createInfo.initialLayout;

    _image = components::device.createImageUnique( createInfo );
    RX_ASSERT( _image.get( ), "Failed to create image" );
    _memory = vk::Initializer::allocateMemoryUnique( _image.get( ) );
  }

  void Image::transitionToLayout( vk::ImageLayout layout )
  {
    auto barrierInfo = vk::Helper::getImageMemoryBarrierInfo( _image.get( ), _layout, layout );

    CommandBuffer commandBuffer;
    commandBuffer.init( components::graphicsCmdPool );
    commandBuffer.begin( );

    commandBuffer.get( 0 ).pipelineBarrier( std::get<1>( barrierInfo ), // srcStageMask
                                            std::get<2>( barrierInfo ), // dstStageMask
                                            vk::DependencyFlagBits::eByRegion,
                                            0,
                                            nullptr,
                                            0,
                                            nullptr,
                                            1,
                                            &std::get<0>( barrierInfo ) ); // barrier

    commandBuffer.end( );
    commandBuffer.submitToQueue( components::graphicsQueue );

    _layout = layout;
  }

  void Image::transitionToLayout( vk::ImageLayout layout, vk::CommandBuffer commandBuffer )
  {
    auto barrierInfo = vk::Helper::getImageMemoryBarrierInfo( _image.get( ), _layout, layout );

    commandBuffer.pipelineBarrier( std::get<1>( barrierInfo ), // srcStageMask
                                   std::get<2>( barrierInfo ), // dstStageMask
                                   vk::DependencyFlagBits::eByRegion,
                                   0,
                                   nullptr,
                                   0,
                                   nullptr,
                                   1,
                                   &std::get<0>( barrierInfo ) ); // barrier

    _layout = layout;
  }
} // namespace RAYEX_NAMESPACE
