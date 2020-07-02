#include "Texture.hpp"
#include "Buffer.hpp"
#include "CommandBuffer.hpp"
#include "Components.hpp"
#include "Initializers.hpp"
#include "Destructors.hpp"

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

  void Image::transitionToLayout( const vk::ImageLayout& layout )
  {
    CommandBuffer commandBuffer;
    commandBuffer.init( g_graphicsCmdPool );

    commandBuffer.begin( );
    {
      vk::ImageMemoryBarrier barrier;
      barrier.oldLayout = m_layout;
      barrier.newLayout = layout;
      barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barrier.image = m_image.get( );
      barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
      barrier.subresourceRange.baseMipLevel = 0;
      barrier.subresourceRange.levelCount = 1;
      barrier.subresourceRange.baseArrayLayer = 0;
      barrier.subresourceRange.layerCount = 1;

      vk::PipelineStageFlags sourceStage;
      vk::PipelineStageFlags destinationStage;

      if ( m_layout == vk::ImageLayout::eUndefined && layout == vk::ImageLayout::eTransferDstOptimal )
      {
        //barrier.srcAccessMask = 0;
        barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

        sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
        destinationStage = vk::PipelineStageFlagBits::eTransfer;
      }
      else if ( m_layout == vk::ImageLayout::eTransferDstOptimal && layout == vk::ImageLayout::eShaderReadOnlyOptimal )
      {
        barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
        barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

        sourceStage = vk::PipelineStageFlagBits::eTransfer;
        destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
      }
      else
        RX_ERROR( "Failed to transition image layout." );

      commandBuffer.get( 0 ).pipelineBarrier(
        sourceStage,
        destinationStage,
        vk::DependencyFlagBits::eByRegion, // TODO: might be cause of an error
        0,
        nullptr,
        0,
        nullptr,
        1,
        &barrier
      );
    }
    commandBuffer.end( );
    commandBuffer.submitToQueue( g_graphicsQueue );

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