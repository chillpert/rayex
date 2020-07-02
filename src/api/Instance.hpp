#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include "WindowBase.hpp"

namespace rx
{
  class Instance
  {
  public:
    Instance() = default;
    Instance(const std::vector<const char*>& layers, std::vector<const char*>& extensions, bool initialize = true);

    /*
      Initializes the instance object and checks support for any given layer and extension.
      @param layers - All validation layers that should be used.
      @param extensions - All instance extensions that should be used.
      Note: The given vector with the extensions will be appended by ones that are required by the window.
    */
    void init(const std::vector<const char*>& layers, std::vector<const char*>& extensions);

  private:
    void checkLayersSupport(const std::vector<const char*>& layers);
    void checkExtensionsSupport(const std::vector<const char*>& extensions);

    // Returns the version of the Vulkan SDK installed on the machine. 
    // Note: It can not retrieve patch version, only major and minor version.
    uint32_t getApiVersion();

    vk::UniqueInstance m_instance;
  };
}

#endif // INSTANCE_HPP