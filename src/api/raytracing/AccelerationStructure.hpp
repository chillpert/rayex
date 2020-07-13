#ifndef ACCELERATION_STRUCTURE_HPP
#define ACCELERATION_STRUCTURE_HPP

#include "stdafx.hpp"
#include "Components.hpp"

namespace rx
{
  struct AccelerationStructure
  {
    vk::AccelerationStructureKHR as;
    vk::DeviceMemory memory;

    void destroy( )
    {
      if ( as )
        g_device.destroyAccelerationStructureKHR( as );

      if ( memory )
        g_device.freeMemory( memory );
    }
  };

  struct Tlas
  {
    AccelerationStructure as;
    vk::BuildAccelerationStructureFlagsKHR flags;
  };

  struct Blas
  {
    AccelerationStructure as;
    vk::BuildAccelerationStructureFlagsKHR flags;                                          // specifying additional parameters for acceleration structure builds

    std::vector<vk::AccelerationStructureCreateGeometryTypeInfoKHR> asCreateGeometryInfo;  // specifies the shape of geometries that will be built into an acceleration structure
    std::vector<vk::AccelerationStructureGeometryKHR> asGeometry;                          // data used to build acceleration structure geometry
    std::vector<vk::AccelerationStructureBuildOffsetInfoKHR> asBuildOffsetInfo;
  };

  // This is an instance of a BLAS.
  struct BlasInstance
  {
    uint32_t blasId = 0;                      // Index of the BLAS in m_blas
    uint32_t instanceId = 0;                  // Instance Index (gl_InstanceID)
    uint32_t hitGroupId = 0;                  // Hit group index in the SBT
    uint32_t mask = 0xFF;                     // Visibility mask, will be AND-ed with ray mask
    vk::GeometryInstanceFlagsKHR flags = { vk::GeometryInstanceFlagBitsKHR::eTriangleFacingCullDisable };
    glm::mat4 transform = glm::fmat4( 1.0f ); // Identity
  };
}

#endif // ACCELERATION_STRUCTURE_HPP