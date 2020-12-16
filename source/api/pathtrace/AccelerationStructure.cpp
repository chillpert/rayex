#include "api/pathtrace/AccelerationStructure.hpp"

#include "api/Components.hpp"

namespace RAYEX_NAMESPACE
{
  void AccelerationStructure::destroy( )
  {
    if ( as )
    {
      components::device.destroyAccelerationStructureKHR( as );
    }

    if ( buffer )
    {
      components::device.destroyBuffer( buffer );
    }

    if ( memory )
    {
      components::device.freeMemory( memory );
    }
  }
} // namespace RAYEX_NAMESPACE
