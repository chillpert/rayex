#include "api/image/Texture.hpp"

#include "api/Components.hpp"
#include "api/buffers/Buffer.hpp"
#include "api/utility/Helpers.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "ktx/ktx.h"
#include "stb/stb_image.h"

namespace RAYEX_NAMESPACE
{
  void Texture::init( std::string_view path )
  {
    _path = path;

    std::string fullPath = components::assetsPath + std::string( path );

    int width;
    int height;
    int channels;
    stbi_uc* pixels = stbi_load( fullPath.c_str( ), &width, &height, &channels, STBI_rgb_alpha );

    if ( pixels == nullptr )
    {
      RX_ERROR( "Failed to load texture" );
    }

    vk::DeviceSize size = width * height * 4;

    // Set up the staging buffer.
    Buffer stagingBuffer( size,
                          vk::BufferUsageFlagBits::eTransferSrc,
                          { components::graphicsFamilyIndex },
                          vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );

    stagingBuffer.fill<stbi_uc>( pixels );

    stbi_image_free( pixels );

    auto imageCreateInfo = vk::Helper::getImageCreateInfo( vk::Extent3D { static_cast<uint32_t>( width ), static_cast<uint32_t>( height ), 1 } );
    Image::init( imageCreateInfo );

    transitionToLayout( vk::ImageLayout::eTransferDstOptimal );
    stagingBuffer.copyToImage( _image.get( ), _extent );
    transitionToLayout( vk::ImageLayout::eShaderReadOnlyOptimal );

    _imageView = vkCore::initImageViewUnique( vk::Helper::getImageViewCreateInfo( _image.get( ), _format ) );

    /*
    _path = path;

    ktxTexture* texture = nullptr;

    std::string fullPath = components::assetsPath + std::string( path );

    if ( ktxTexture_CreateFromNamedFile( fullPath.c_str( ), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &texture ) != KTX_SUCCESS )
    {
      RX_FATAL( "Failed to load KTX file from: ", fullPath );
    }

    ktx_uint8_t* textureData = texture->pData;
    ktx_size_t textureSize   = texture->dataSize;
    uint32_t width           = texture->baseWidth;
    uint32_t height          = texture->baseHeight;
    uint32_t numLevels       = 1;

    // Set up the staging buffer.
    Buffer stagingBuffer( textureSize,
                          vk::BufferUsageFlagBits::eTransferSrc,
                          { components::graphicsFamilyIndex },
                          vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );

    stagingBuffer.fill<ktx_uint8_t>( textureData );

    auto imageCreateInfo = vk::Helper::getImageCreateInfo( vk::Extent3D { static_cast<uint32_t>( width ), static_cast<uint32_t>( height ), 1 } );
    Image::init( imageCreateInfo );

    transitionToLayout( vk::ImageLayout::eTransferDstOptimal );
    stagingBuffer.copyToImage( _image.get( ), _extent );
    transitionToLayout( vk::ImageLayout::eShaderReadOnlyOptimal );

    _imageView = vkCore::initImageViewUnique( vk::Helper::getImageViewCreateInfo( _image.get( ), _format ) );

    ktxTexture_Destroy( texture );
    */
  }
} // namespace RAYEX_NAMESPACE
