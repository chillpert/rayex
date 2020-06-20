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

    // Creates a single bottom level AS.
    void initialize(BottomLevelASInfo& info, const std::shared_ptr<Model> model, std::vector<BottomLevelAS>& blas);
    // Creates bottom level AS for all given models.
    friend void initBottomLevelAS_(BottomLevelASInfo& info, const std::unordered_map<std::string, std::shared_ptr<Model>>& models, std::vector<BottomLevelAS>& blas_);

  private:
    BottomLevelASInfo m_info;

    vk::AccelerationStructureKHR as;
    vk::DeviceMemory asMemory;
  };
}

#endif // BOTTOM_LEVEL_AS_HPP