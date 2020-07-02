#ifndef TOP_LEVEL_AS_HPP
#define TOP_LEVEL_AS_HPP

#include "Model.hpp"
#include "NodeBase.hpp"

namespace rx
{
  class TopLevelAS
  {
  public:
    TopLevelAS( ) = default;
    TopLevelAS( std::vector<std::shared_ptr<GeometryNodeBase>> nodes, vk::BuildAccelerationStructureFlagsKHR flags, bool initialize = true );
    ~TopLevelAS( );

    inline const vk::AccelerationStructureKHR get( ) const { return m_as; }

    // Creates top level AS for all given models.
    void init( std::vector<std::shared_ptr<GeometryNodeBase>> nodes, vk::BuildAccelerationStructureFlagsKHR flags );
    void destroy( );

  private:
    vk::AccelerationStructureKHR m_as;
    vk::DeviceMemory m_memory;
    uint64_t m_handle;
  };
}

#endif // TOP_LEVEL_AS_HPP