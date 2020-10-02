#include "api/image/Texture.hpp"
#include "api/buffers/Buffer.hpp"
#include "api/buffers/CommandBuffer.hpp"
#include "api/misc/Components.hpp"
#include "api/utility/Initializers.hpp"
#include "api/utility/Destructors.hpp"
#include "api/utility/Helpers.hpp"

namespace RAYEXEC_NAMESPACE
{
  void Image::init( const vk::ImageCreateInfo& createInfo )
  {
    this->extent = createInfo.extent;
    this->format = createInfo.format;
    this->layout = createInfo.initialLayout;

    this->image = g_device.createImageUnique( createInfo );
    RX_ASSERT( this->image, "Failed to create image" );
    this->memory = vk::Initializer::allocateMemoryUnique( this->image.get( ) );
  }

  void Image::transitionToLayout( vk::ImageLayout layout )
  {
    auto barrierInfo = vk::Helper::getImageMemoryBarrierInfo( this->image.get( ), this->layout, layout );

    CommandBuffer commandBuffer;
    commandBuffer.init( g_graphicsCmdPool );
    commandBuffer.begin( );

    commandBuffer.get( 0 ).pipelineBarrier( std::get<1>( barrierInfo ),        // srcStageMask
                                            std::get<2>( barrierInfo ),        // dstStageMask
                                            vk::DependencyFlagBits::eByRegion,
                                            0,
                                            nullptr,
                                            0,
                                            nullptr,
                                            1,
                                            &std::get<0>( barrierInfo ) );     // barrier

    commandBuffer.end( );
    commandBuffer.submitToQueue( g_graphicsQueue );

    this->layout = layout;
  }

  void Image::transitionToLayout( vk::ImageLayout layout, vk::CommandBuffer commandBuffer )
  {
    auto barrierInfo = vk::Helper::getImageMemoryBarrierInfo( this->image.get( ), this->layout, layout );

    commandBuffer.pipelineBarrier( std::get<1>( barrierInfo ),        // srcStageMask
                                   std::get<2>( barrierInfo ),        // dstStageMask
                                   vk::DependencyFlagBits::eByRegion,
                                   0,
                                   nullptr,
                                   0,
                                   nullptr,
                                   1,
                                   &std::get<0>( barrierInfo ) );     // barrier

    this->layout = layout;
  }

  vk::Format Image::findSupportedFormat( vk::PhysicalDevice physicalDevice, const std::vector<vk::Format>& formatsToTest, vk::FormatFeatureFlagBits features, vk::ImageTiling tiling )
  {
    for ( vk::Format format : formatsToTest )
    {
      auto props = physicalDevice.getFormatProperties( format );

      if ( tiling == vk::ImageTiling::eLinear && ( props.linearTilingFeatures & features ) == features )
        return format;

      else if ( tiling == vk::ImageTiling::eOptimal && ( props.optimalTilingFeatures & features ) == features )
        return format;
    }

    RX_ERROR( "Failed to retrieve any supported image format." );

    return vk::Format::eUndefined;
  }
}