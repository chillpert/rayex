#include "TopLevelAS.hpp"

namespace RX
{
  TopLevelAS::~TopLevelAS()
  {
    if (accelerationStructure)
      destroy();
  }

  void TopLevelAS::destroy()
  {
    device.destroyAccelerationStructureKHR(accelerationStructure, nullptr, dispatchLoaderDynamic);
    accelerationStructure = nullptr;
  }

  void TopLevelAS::initialize(const std::vector<std::shared_ptr<ModelBase>>& models)
  {
    
  }

  void TopLevelAS::build(std::vector<TopLevelAS>& blas_)
  {
    
  }
}