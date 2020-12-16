#pragma once

#include "api/image/Image.hpp"

namespace RAYEX_NAMESPACE
{
  /// A specialization class for creating textures using the sbt_image header.
  /// @ingroup API
  class Texture : public Image
  {
  public:
    auto getImageView( ) const -> vk::ImageView { return _imageView.get( ); }

    auto getPath( ) const -> const std::string& { return _path; }

    /// Creates the texture.
    /// @param path The relative path to the texture file.
    void init( std::string_view path );

  private:
    std::string _path; ///< The relative path to the texture file.

    vk::UniqueImageView _imageView;
  };
} // namespace RAYEX_NAMESPACE
