#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Image.hpp"

namespace rx
{
  /// A helper class for creating ready-to-use textures.
  /// @ingroup API
  class Texture : public Image
  {
  public:
    RX_API Texture( ) = default;

    /// @param path The relative path to the texture file.
    /// @param initialize If true, the texture will be initialized right away without an additional call to init().
    RX_API Texture( const std::string& path, bool initialize = true );

    /// @return Returns the texture's image view.
    inline const vk::ImageView getImageView( ) const { return m_imageView.get( ); }
    
    /// @return Returns the texture's sampler.
    inline const vk::Sampler getSampler( ) const { return m_sampler.get( ); }
    
    /// @return Returns the relative path of the texture file.
    inline const std::string& getPath( ) const { return m_path; }

    /// Creates the texture.
    /// @param path The relative path to the texture file.
    /// @todo Latest changes to this class might be causing errors once texture are working again.
    void init( const std::string& path );

  private:
    std::string m_path; ///< The relative path to the texture file.

    vk::UniqueImageView m_imageView; ///< The texture's Vulkan image view with a unique handle.
    vk::UniqueSampler m_sampler; ///< The texture's Vulkan sampler with a unique handle.
  };
}

namespace std
{
  /// @cond INTERNAL
  template<> struct hash<rx::Texture>
  {
    size_t operator()( const std::shared_ptr<rx::Texture> texture ) const { return hash<std::string>( )( texture->getPath( ) ); }
  };
  /// @endcond
}

#endif // TEXTURE_HPP