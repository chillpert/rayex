#pragma once

#include "vkCore/vkCore.hpp"

namespace RAYEX_NAMESPACE
{
  /// A specialization class for creating cubemaps.
  /// @ingroup API
  class Cubemap : public vkCore::Image
  {
  public:
    auto getImageView( ) const -> const vk::ImageView { return _imageView.get( ); }

    auto getSampler( ) const -> const vk::Sampler { return _sampler.get( ); }

    /// Initializes the cubemap.
    /// @param path A path to a ktx cubemap file. If left empty, an empty cubemap will be created instead.
    void init( std::string_view path = std::string_view( ) );

  private:
    vk::UniqueSampler _sampler;
    vk::UniqueImageView _imageView;
  };

} // namespace RAYEX_NAMESPACE