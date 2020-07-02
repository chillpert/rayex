#include "Texture.hpp"
#include "Buffer.hpp"
#include "Helpers.hpp"
#include "Initializers.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace rx
{
  Texture::Texture( const std::string& path, bool initialize )
  {
    if ( initialize )
      init( path );
  }

  void Texture::init( const std::string& path )
  {
    m_path = path;

    int width, height, channels;
    stbi_uc* pixels = stbi_load( path.c_str( ), &width, &height, &channels, STBI_rgb_alpha );

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
    m_image.init( imageCreateInfo );

    m_image.transitionToLayout( vk::ImageLayout::eTransferDstOptimal );
    stagingBuffer.copyToImage( m_image );
    m_image.transitionToLayout( vk::ImageLayout::eShaderReadOnlyOptimal );

    m_imageView = vk::Initializer::createImageViewUnique( m_image.get( ), m_image.getFormat( ) );

    auto samplerCreateInfo = vk::Helper::getSamplerCreateInfo( );
    m_sampler = vk::Initializer::createSamplerUnique( samplerCreateInfo );
  }
}