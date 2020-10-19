#ifndef ACCELERATION_STRUCTURE_HPP
#define ACCELERATION_STRUCTURE_HPP

#include "api/Components.hpp"
#include "pch/stdafx.hpp"

namespace RAYEX_NAMESPACE
{
  /// A wrapper for a Vulkan acceleration Structure.
  /// @ingroup API
  struct AccelerationStructure
  {
    vk::AccelerationStructureKHR as; ///< The Vulkan acceleration structure.
    vk::DeviceMemory memory;         ///< The acceleration structure's memory.

    /// Used to destroy the acceleration structure and free its memory.
    void destroy( )
    {
      if ( as )
        g_device.destroyAccelerationStructureKHR( as );

      if ( memory )
        g_device.freeMemory( memory );
    }
  };

  /// A wrapper for a top level acceleration structure.
  /// @ingroup API
  struct Tlas
  {
    AccelerationStructure as;                     ///< The RAYEX_NAMESPACE::AccelerationStructure object containing the Vulkan acceleration structure.
    vk::BuildAccelerationStructureFlagsKHR flags; ///< The top level acceleration structure's build flags.
  };

  /// A wrapper for a bottom level acceleration structure.
  /// @ingroup API
  struct Blas
  {
    AccelerationStructure as;                     ///< The RAYEX_NAMESPACE::AccelerationStructure object containing the Vulkan acceleration structure.
    vk::BuildAccelerationStructureFlagsKHR flags; ///< The top level acceleration structure's build flags.

    std::vector<vk::AccelerationStructureCreateGeometryTypeInfoKHR> asCreateGeometryInfo; ///< Specifies the shape of geometries that will be built into an acceleration structure.
    std::vector<vk::AccelerationStructureGeometryKHR> asGeometry;                         ///< Data used to build acceleration structure geometry.
    std::vector<vk::AccelerationStructureBuildOffsetInfoKHR> asBuildOffsetInfo;           ///< The offset between acceleration structures when building.
  };

  /// An instance of a bottom level acceleration structure.
  /// @ingroup API
  struct BlasInstance
  {
    uint32_t blasId                    = 0;                                                               ///< The index of the bottom level acceleration structure in blas_.
    uint32_t instanceId                = 0;                                                               ///< The instance index (gl_InstanceID).
    uint32_t hitGroupId                = 0;                                                               ///< The hit group index in the shader binding table.
    uint32_t mask                      = 0xFF;                                                            ///< The visibility mask, will be AND-ed with the ray mask.
    vk::GeometryInstanceFlagsKHR flags = { vk::GeometryInstanceFlagBitsKHR::eTriangleFacingCullDisable }; ///< The geometry display options.
    glm::mat4 transform                = glm::fmat4( 1.0f );                                              ///< The world transform matrix of the bottom level acceleration structure instance.
  };
} // namespace RAYEX_NAMESPACE

#endif // ACCELERATION_STRUCTURE_HPP
