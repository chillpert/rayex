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
        g_device.destroyAccelerationStructureKHR( as, nullptr, *g_dispatchLoaderDynamic );

      if ( memory )
        g_device.freeMemory( memory );
    }
  };

  struct Blas
  {
    AccelerationStructure as;
    vk::BuildAccelerationStructureFlagsKHR flags;                                          // specifying additional parameters for acceleration structure builds

    std::vector<vk::AccelerationStructureCreateGeometryTypeInfoKHR> asCreateGeometryInfo;  // specifies the shape of geometries that will be built into an acceleration structure
    std::vector<vk::AccelerationStructureGeometryKHR> asGeometry;                          // data used to build acceleration structure geometry
    std::vector<vk::AccelerationStructureBuildOffsetInfoKHR> asBuildOffsetInfo;
  };
}

#endif // ACCELERATION_STRUCTURE_HPP