#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "api/image/Image.hpp"

namespace RAYEX_NAMESPACE
{
  /// A helper class for creating ready-to-use textures.
  /// @ingroup API
  /// @todo textureCounter is being incremented always, even if it is not a new texture. This class should actually look inside Api::textures or sth to assign the correct index.
  class Texture : public Image
  {
  public:
    Texture( );

    /// @param path The relative path to the texture file.
    /// @param initialize If true, the texture will be initialized right away without an additional call to init().
    Texture( std::string_view path, bool initialize = true );

    /// @return Returns the texture's image view.
    auto getImageView( ) -> vk::ImageView { return _imageView.get( ); }

    /// @return Returns the texture's sampler.
    auto getSampler( ) -> vk::Sampler { return _sampler.get( ); }

    /// @return Returns the relative path of the texture file.
    auto getPath( ) const -> const std::string& { return _path; }

    /// Creates the texture.
    /// @param path The relative path to the texture file.
    /// @todo Latest changes to this class might be causing errors once texture are working again.
    void init( std::string_view path );

    uint32_t _offset;

  private:
    std::string _path; ///< The relative path to the texture file.

    vk::UniqueImageView _imageView; ///< The texture's Vulkan image view with a unique handle.
    vk::UniqueSampler _sampler;     ///< The texture's Vulkan sampler with a unique handle.

    static uint32_t _textureCounter;
  };
} // namespace RAYEX_NAMESPACE

namespace std
{
  /// @cond INTERNAL
  template <>
  struct hash<RAYEX_NAMESPACE::Texture>
  {
    auto operator( )( const std::shared_ptr<RAYEX_NAMESPACE::Texture> texture ) const -> size_t { return hash<std::string>( )( texture->getPath( ) ); }
  };
  /// @endcond
} // namespace std

#endif // TEXTURE_HPP
