#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Image.hpp"

namespace rx
{
  class Texture
  {
  public:
    RX_API Texture( ) = default;
    RX_API Texture( const std::string& path, bool initialize = true );

    inline const vk::Image getImage( ) const { return m_image.get( ); }
    inline const vk::ImageView getImageView( ) const { return m_imageView.get( ); }
    inline const vk::Sampler getSampler( ) const { return m_sampler.get( ); }
    inline const std::string& getPath( ) const { return m_path; }

    void init( const std::string& path );

  private:
    Image m_image;
    std::string m_path;

    vk::UniqueImageView m_imageView;
    vk::UniqueSampler m_sampler;
  };
}

namespace std
{
  template<> struct hash<rx::Texture>
  {
    size_t operator()( const std::shared_ptr<rx::Texture> texture ) const { return hash<std::string>( )( texture->getPath( ) ); }
  };
}

#endif // TEXTURE_HPP