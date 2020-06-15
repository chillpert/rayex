#ifndef TOP_LEVEL_AS_HPP
#define TOP_LEVEL_AS_HPP

#include "Model.hpp"

namespace RX
{
  class TopLevelAS
  {
  public:
    ~TopLevelAS();
    void destroy();

    static void initialize(const std::vector<std::shared_ptr<ModelBase>>& models);
    static void build(std::vector<TopLevelAS>& tlas_);

  private:
    vk::AccelerationStructureKHR accelerationStructure;
    vk::DeviceMemory accelerationStructureAllocation;
    vk::AccelerationStructureCreateInfoKHR asInfo;
    vk::BuildAccelerationStructureFlagsKHR flags;

    vk::DispatchLoaderDynamic dispatchLoaderDynamic;
    vk::Device device;
  };
}

#endif // TOP_LEVEL_AS_HPP