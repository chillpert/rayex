#include "api/image/Cubemap.hpp"

#include "api/Components.hpp"
#include "api/buffers/Buffer.hpp"
#include "api/buffers/CommandBuffer.hpp"
#include "api/utility/Helpers.hpp"
#include "api/utility/Initializers.hpp"
#include "ktx/ktx.h"

namespace RAYEX_NAMESPACE
{
  void Cubemap::init( std::string_view path )
  {
    ktxTexture* texture = nullptr;

    ktx_uint8_t data         = '0';
    ktx_uint8_t* textureData = &data;
    ktx_size_t textureSize   = 16;
    uint32_t width           = 1;
    uint32_t height          = 1;
    uint32_t numLevels       = 1;

    if ( !path.empty( ) )
    {
      std::string fullPath = components::assetsPath + std::string( path );

      if ( ktxTexture_CreateFromNamedFile( fullPath.c_str( ), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &texture ) != KTX_SUCCESS )
      {
        RX_FATAL( "Failed t o load KTX file from: ", fullPath );
      }

      textureData = texture->pData;
      textureSize = texture->dataSize;
      width       = texture->baseWidth;
      height      = texture->baseHeight;
    }

    // Set up the staging buffer.
    Buffer stagingBuffer( textureSize,
                          vk::BufferUsageFlagBits::eTransferSrc,
                          { components::graphicsFamilyIndex },
                          vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );

    vk::MemoryRequirements reqs = components::device.getBufferMemoryRequirements( stagingBuffer.get( ) );

    // Copy to staging buffer
    stagingBuffer.fill<ktx_uint8_t>( textureData );

    // Init image
    vk::Extent3D extent( width, height, 1 );

    auto imageCreateInfo        = vk::Helper::getImageCreateInfo( extent );
    imageCreateInfo.arrayLayers = 6;
    imageCreateInfo.flags       = vk::ImageCreateFlagBits::eCubeCompatible;
    imageCreateInfo.mipLevels   = numLevels;
    imageCreateInfo.format      = vk::Format::eR8G8B8A8Unorm; // @todo add format support handling

    Image::init( imageCreateInfo );

    // Image create info
    auto imageViewCreateInfo                        = vk::Helper::getImageViewCreateInfo( _image.get( ), _format, vk::ImageViewType::eCube );
    imageViewCreateInfo.subresourceRange.layerCount = 6;
    imageViewCreateInfo.subresourceRange.levelCount = numLevels;
    imageViewCreateInfo.components                  = { vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA };

    // Create buffer copy regions
    std::vector<vk::BufferImageCopy> bufferCopyRegions;
    bufferCopyRegions.reserve( 6 * numLevels );

    for ( uint32_t face = 0; face < 6; ++face )
    {
      for ( uint32_t mipLevel = 0; mipLevel < numLevels; ++mipLevel )
      {
        ktx_size_t offset = 0;
        if ( texture != nullptr )
        {
          RX_ASSERT( ktxTexture_GetImageOffset( texture, mipLevel, 0, face, &offset ) == KTX_SUCCESS, "KTX: Failed to get image offset." );
        }
        vk::BufferImageCopy bufferCopyRegion             = { };
        bufferCopyRegion.imageSubresource.aspectMask     = vk::ImageAspectFlagBits::eColor;
        bufferCopyRegion.imageSubresource.mipLevel       = mipLevel;
        bufferCopyRegion.imageSubresource.baseArrayLayer = face;
        bufferCopyRegion.imageSubresource.layerCount     = 1;
        bufferCopyRegion.imageExtent.width               = width >> mipLevel;
        bufferCopyRegion.imageExtent.height              = height >> mipLevel;
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
    samplerCreateInfo.maxLod        = static_cast<float>( numLevels );
    samplerCreateInfo.borderColor   = vk::BorderColor::eFloatOpaqueWhite;
    samplerCreateInfo.maxAnisotropy = 1.0F;
    samplerCreateInfo.compareOp     = vk::CompareOp::eNever;

    _sampler = vkCore::initSamplerUnique( samplerCreateInfo );

    // Init image view
    _imageView = vkCore::initImageViewUnique( imageViewCreateInfo );

    // Clean up
    if ( texture != nullptr )
    {
      ktxTexture_Destroy( texture );
    }
  }
} // namespace RAYEX_NAMESPACE
