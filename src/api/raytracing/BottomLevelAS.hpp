#ifndef BOTTOM_LEVEL_AS_HPP
#define BOTTOM_LEVEL_AS_HPP

#include "Model.hpp"
#include "Queue.hpp"

namespace RX
{
  struct BottomLevelASInfo
  {
    vk::DispatchLoaderDynamic dispatchLoaderDynamic;
    vk::Device device;
    vk::PhysicalDevice physicalDevice;
    vk::BuildAccelerationStructureFlagsKHR flags;
    std::shared_ptr<Queue> queue;
  };

  class BottomLevelAS
  {
  public:
    BottomLevelAS() = default;
    BottomLevelAS(BottomLevelASInfo& info);
    ~BottomLevelAS();
    void destroy();

    inline BottomLevelASInfo& getInfo() { return m_info; }

    friend void initBottomLevelAS_(BottomLevelASInfo& info, const std::vector<std::shared_ptr<Model>>& models);

    BottomLevelAS& operator=(const std::shared_ptr<Model> model);
    void wavefrontToBottomLevelAS(std::shared_ptr<Model> model);

  private:
    BottomLevelASInfo m_info;

    vk::AccelerationStructureKHR accelerationStructure;
    vk::DeviceMemory accelerationStructureAllocation;
    
    std::vector<vk::AccelerationStructureCreateGeometryTypeInfoKHR> asCreateGeometryInfo;
    std::vector<vk::AccelerationStructureGeometryKHR> asGeometry;
    std::vector<vk::AccelerationStructureBuildOffsetInfoKHR> asBuildOffsetInfo;
  };
}

#endif // BOTTOM_LEVEL_AS_HPP