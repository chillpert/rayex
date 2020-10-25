#include "api/image/Texture.hpp"

#include "api/buffers/Buffer.hpp"
#include "api/utility/Helpers.hpp"
#include "api/utility/Initializers.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

    _imageView = vk::Initializer::initImageViewUnique( _image.get( ), _format );

    auto samplerCreateInfo = vk::Helper::getSamplerCreateInfo( );
    _sampler               = vk::Initializer::initSamplerUnique( samplerCreateInfo );
  }
} // namespace RAYEX_NAMESPACE
