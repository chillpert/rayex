#include "Texture.hpp"
#include "Buffer.hpp"
#include "CommandBuffer.hpp"
#include "Components.hpp"
#include "Initializers.hpp"
#include "Destructors.hpp"
#include "Helpers.hpp"

namespace rx
{
  Image::Image( const vk::ImageCreateInfo& createInfo, bool initialize )
  {
    if ( initialize )
      init( createInfo );
  }

  void Image::init( const vk::ImageCreateInfo& createInfo )
  {
    m_extent = createInfo.extent;
    m_format = createInfo.format;
    m_layout = createInfo.initialLayout;

    m_image = g_device.createImageUnique( createInfo );
    if ( !m_image )
      RX_ERROR( "Failed to create image." );

    m_memory = vk::Initializer::allocateMemoryUnique( m_image.get( ) );
  }

  void Image::transitionToLayout( vk::ImageLayout layout )
  {
    auto barrierInfo = vk::Helper::getImageMemoryBarrierInfo( m_image.get( ), m_layout, layout );

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

    m_layout = layout;
  }

  void Image::transitionToLayout( vk::ImageLayout layout, vk::CommandBuffer commandBuffer )
  {
    auto barrierInfo = vk::Helper::getImageMemoryBarrierInfo( m_image.get( ), m_layout, layout );

    commandBuffer.pipelineBarrier( std::get<1>( barrierInfo ),        // srcStageMask
                                   std::get<2>( barrierInfo ),        // dstStageMask
                                   vk::DependencyFlagBits::eByRegion,
                                   0,
                                   nullptr,
                                   0,
                                   nullptr,
                                   1,
                                   &std::get<0>( barrierInfo ) );     // barrier

    m_layout = layout;
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