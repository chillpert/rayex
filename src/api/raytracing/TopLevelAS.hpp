#ifndef TOP_LEVEL_AS_HPP
#define TOP_LEVEL_AS_HPP

#include "Model.hpp"
#include "NodeBase.hpp"

namespace RX
{
  struct TopLevelASInfo
  {
    vk::Device device;
    vk::PhysicalDevice physicalDevice;
    vk::DispatchLoaderDynamic dispatchLoaderDynamic;
    std::vector<std::shared_ptr<GeometryNodeBase>> nodes;
    vk::BuildAccelerationStructureFlagsKHR flags;
  };

  class TopLevelAS
  {
  public:
    ~TopLevelAS();
    void destroy();

    inline vk::AccelerationStructureKHR& get() { return m_as; }

    // Creates top level AS for all given models.
    void initialize(TopLevelASInfo& info);

  private:
    TopLevelASInfo m_info;

    vk::AccelerationStructureKHR m_as;
    vk::DeviceMemory m_memory;
    uint64_t m_handle;
  };
}

#endif // TOP_LEVEL_AS_HPP