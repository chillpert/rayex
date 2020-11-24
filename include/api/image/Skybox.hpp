#pragma once

#include "api/image/Image.hpp"
#include "stb_image.h"

#include <ktx/ktx.h>
#include <ktx/ktxvulkan.h>

namespace rx
{
  class Skybox : public Image
  {
  public:
    /// @return Returns the skybox's image view.
    auto getImageView( ) const -> vk::ImageView { return _imageView.get( ); }

    auto getSampler( ) const -> vk::Sampler { return _sampler.get( ); }

    void init( const std::array<std::string_view, 6>& texturePaths )
    {
      ktxResult result;
      ktxTexture* texture;
      std::string fullPath = components::assetsPath + "models/skybox/cubemap_yokohama_bc3_unorm.ktx";

      if ( ktxTexture_CreateFromNamedFile( fullPath.c_str( ), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &texture ) != KTX_SUCCESS )
      {
        RX_FATAL( "Failed to load KTX file from: ", fullPath );
      }

      auto textureData = texture->pData;
      auto textureSize = texture->dataSize;

      // Set up the staging buffer.
      Buffer stagingBuffer( textureSize,
                            vk::BufferUsageFlagBits::eTransferSrc,
                            { components::graphicsFamilyIndex },
                            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );

      vk::MemoryRequirements reqs = components::device.getBufferMemoryRequirements( stagingBuffer.get( ) );

      // Copy to staging buffer
      stagingBuffer.fill<ktx_uint8_t>( textureData );

      // Init image
      vk::Extent3D extent( static_cast<uint32_t>( texture->baseWidth ), static_cast<uint32_t>( texture->baseHeight ), 1 );

      auto imageCreateInfo        = vk::Helper::getImageCreateInfo( extent );
      imageCreateInfo.arrayLayers = 6;
      imageCreateInfo.flags       = vk::ImageCreateFlagBits::eCubeCompatible;
      imageCreateInfo.mipLevels   = texture->numLevels;
      imageCreateInfo.format      = vk::Format::eBc2UnormBlock; // @todo add format support handling

      Image::init( imageCreateInfo );

      // Image create info
      auto imageViewCreateInfo                        = vk::Helper::getImageViewCreateInfo( _image.get( ), _format, vk::ImageViewType::eCube );
      imageViewCreateInfo.subresourceRange.layerCount = 6;
      imageViewCreateInfo.subresourceRange.levelCount = texture->numLevels;
      imageViewCreateInfo.components                  = { vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA };

      // Create buffer copy regions
      std::vector<vk::BufferImageCopy> bufferCopyRegions;
      bufferCopyRegions.reserve( 6 * texture->numLevels );

      for ( uint32_t face = 0; face < 6; ++face )
      {
        for ( uint32_t mipLevel = 0; mipLevel < texture->numLevels; ++mipLevel )
        {
          ktx_size_t offset;
          RX_ASSERT( ktxTexture_GetImageOffset( texture, mipLevel, 0, face, &offset ) == KTX_SUCCESS, "KTX: Failed to get image offset." );
          vk::BufferImageCopy bufferCopyRegion             = { };
          bufferCopyRegion.imageSubresource.aspectMask     = vk::ImageAspectFlagBits::eColor;
          bufferCopyRegion.imageSubresource.mipLevel       = mipLevel;
          bufferCopyRegion.imageSubresource.baseArrayLayer = face;
          bufferCopyRegion.imageSubresource.layerCount     = 1;
          bufferCopyRegion.imageExtent.width               = texture->baseWidth >> mipLevel;
          bufferCopyRegion.imageExtent.height              = texture->baseHeight >> mipLevel;
          bufferCopyRegion.imageExtent.depth               = 1;
          bufferCopyRegion.bufferOffset                    = offset;
          bufferCopyRegions.push_back( bufferCopyRegion );
        }
      }

      // Fill the actual image.
      transitionToLayout( vk::ImageLayout::eTransferDstOptimal, &imageViewCreateInfo.subresourceRange );

      CommandBuffer commandBuffer( components::graphicsCmdPool );
      commandBuffer.begin( );
      {
        commandBuffer.get( 0 ).copyBufferToImage( stagingBuffer.get( ),
                                                  _image.get( ),
                                                  vk::ImageLayout::eTransferDstOptimal,
                                                  static_cast<uint32_t>( bufferCopyRegions.size( ) ),
                                                  bufferCopyRegions.data( ) ); // CMD
      }
      commandBuffer.end( );
      commandBuffer.submitToQueue( components::graphicsQueue );

      transitionToLayout( vk::ImageLayout::eShaderReadOnlyOptimal, &imageViewCreateInfo.subresourceRange );

      // Init sampler
      auto samplerCreateInfo          = vk::Helper::getSamplerCreateInfo( );
      samplerCreateInfo.addressModeU  = vk::SamplerAddressMode::eClampToEdge;
      samplerCreateInfo.addressModeV  = vk::SamplerAddressMode::eClampToEdge;
      samplerCreateInfo.addressModeW  = vk::SamplerAddressMode::eClampToEdge;
      samplerCreateInfo.minLod        = 0.0F;
      samplerCreateInfo.maxLod        = static_cast<float>( texture->numLevels );
      samplerCreateInfo.borderColor   = vk::BorderColor::eFloatOpaqueWhite;
      samplerCreateInfo.maxAnisotropy = 1.0F;
      samplerCreateInfo.compareOp     = vk::CompareOp::eNever;

      _sampler = vk::Initializer::initSamplerUnique( samplerCreateInfo );

      // Init image view
      _imageView = vk::Initializer::initImageViewUnique( imageViewCreateInfo );

      // Clean up
      ktxTexture_Destroy( texture );
    }

  private:
    vk::UniqueImageView _imageView;
    vk::UniqueSampler _sampler;
  };
} // namespace rx
