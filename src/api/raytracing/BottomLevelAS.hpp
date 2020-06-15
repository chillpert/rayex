#ifndef BOTTOM_LEVEL_AS_HPP
#define BOTTOM_LEVEL_AS_HPP

#include "Base.hpp"
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

    friend void initBottomLevelAS_(BottomLevelASInfo& info, const std::unordered_set<std::shared_ptr<ModelBase>>& models);

    BottomLevelAS& operator=(const std::shared_ptr<ModelBase> model);
    friend void wavefrontToBottomLevelAS(BottomLevelAS& blas, std::shared_ptr<ModelBase> model);

  private:
    BottomLevelASInfo m_info;

    vk::AccelerationStructureKHR accelerationStructure;

    std::vector<vk::AccelerationStructureCreateGeometryTypeInfoKHR> asCreateGeometryInfo;
    std::vector<vk::AccelerationStructureGeometryKHR> asGeometry;
    std::vector<vk::AccelerationStructureBuildOffsetInfoKHR> asBuildOffsetInfo;
  };
}

#endif // BOTTOM_LEVEL_AS_HPP