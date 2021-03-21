#include "api/Components.hpp"

namespace RAYEX_NAMESPACE
{
  namespace components
  {
    int frameCount = -1;
    std::string assetsPath;
    uint32_t geometryIndex = 0;
    uint32_t materialIndex = 0;
    uint32_t textureIndex  = 0;

    std::vector<Material> _materials;

  } // namespace components
} // namespace RAYEX_NAMESPACE
