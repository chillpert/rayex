#ifndef BOTTOM_LEVEL_AS_HPP
#define BOTTOM_LEVEL_AS_HPP

#include "Base.hpp"

namespace rx
{
  class BottomLevelAS
  {
  public:
    ~BottomLevelAS( );
    void destroy( );

    inline vk::AccelerationStructureKHR& get( ) { return m_as; }

    // Creates bottom level AS for all given models.
    friend void initBottomLevelAS_( vk::BuildAccelerationStructureFlagsKHR flags, const std::vector<std::shared_ptr<Model>>& models, std::vector<BottomLevelAS>& blas_ );
    // Builds all bottom level AS for all given models.
    friend void buildBottomLevelAS_( vk::BuildAccelerationStructureFlagsKHR flags, const std::vector<std::shared_ptr<Model>>& models, std::vector<BottomLevelAS>& blas_ );

  private:
    vk::AccelerationStructureKHR m_as;
    vk::DeviceMemory m_memory;
    uint64_t m_handle;
  };
}

#endif // BOTTOM_LEVEL_AS_HPP