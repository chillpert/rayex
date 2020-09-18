#include "api/image/Texture.hpp"
#include "api/buffers/Buffer.hpp"
#include "api/utility/Helpers.hpp"
#include "api/utility/Initializers.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace RENDERER_NAMESPACE
{
  uint32_t Texture::textureCounter = 0;

  Texture::Texture( ) :
    offset( this->textureCounter++ ) { }

  Texture::Texture( const std::string& path, bool initialize ) :
    offset( this->textureCounter++ )
  {
    if ( initialize )
      init( path );
  }

  void Texture::init( const std::string& path )
  {
    this->path = path;

    std::string fullPath = g_resourcePath + path;

    int width, height, channels;
    stbi_uc* pixels = stbi_load( fullPath.c_str( ), &width, &height, &channels, STBI_rgb_alpha );

    if ( !pixels )
      RX_ERROR( "Failed to load texture" );

    vk::DeviceSize size = width * height * 4;

    // Set up the staging buffer.
    Buffer stagingBuffer( size,
                          vk::BufferUsageFlagBits::eTransferSrc,
                          { g_graphicsFamilyIndex },
                          vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );

    stagingBuffer.fill<stbi_uc>( pixels );

    stbi_image_free( pixels );

    auto imageCreateInfo = vk::Helper::getImageCreateInfo( vk::Extent3D { static_cast<uint32_t>( width ), static_cast<uint32_t>( height ), 1 } );
    Image::init( imageCreateInfo );

    transitionToLayout( vk::ImageLayout::eTransferDstOptimal );
    stagingBuffer.copyToImage( *this );
    transitionToLayout( vk::ImageLayout::eShaderReadOnlyOptimal );

    this->imageView = vk::Initializer::createImageViewUnique( this->image.get( ), this->format );

    auto samplerCreateInfo = vk::Helper::getSamplerCreateInfo( );
    this->sampler = vk::Initializer::createSamplerUnique( samplerCreateInfo );
  }
}