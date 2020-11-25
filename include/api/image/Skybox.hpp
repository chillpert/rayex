#pragma once

#include "api/image/Image.hpp"

namespace rx
{
  class Skybox : public Image
  {
  public:
    /// @return Returns the skybox's image view.
    auto getImageView( ) const -> vk::ImageView { return _imageView.get( ); }

    /// @return Returns the skybox's sampler.
    auto getSampler( ) const -> vk::Sampler { return _sampler.get( ); }

    /// Initializes the skybox cubemap.
    /// @param path A path to a ktx cubemap file. If left empty, a black cubemap will be created.
    void init( std::string_view path = std::string_view( ) );

  private:
    vk::UniqueSampler _sampler;     ///< The skybox's sampler.
    vk::UniqueImageView _imageView; ///< THe skybox's image view.
  };
} // namespace rx
