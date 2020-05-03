#include "api/VulkanApi.hpp"

using namespace RX;

int main()
{
  std::unique_ptr<Api> api = std::make_unique<VulkanApi>();

  return 0;
}