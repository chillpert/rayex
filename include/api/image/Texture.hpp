#pragma once

#include "api/image/Image.hpp"

namespace RAYEX_NAMESPACE
{
  /// A helper class for creating ready-to-use textures.
  /// @ingroup API
  /// @todo textureCounter is being incremented always, even if it is not a new texture. This class should actually look inside Api::textures or sth to assign the correct index.
  class Texture : public Image
  {
  public:
    /// @return Returns the texture's image view.
    auto getImageView( ) const -> vk::ImageView { return _imageView.get( ); }

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
  };
} // namespace RAYEX_NAMESPACE
