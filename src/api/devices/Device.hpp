#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "Instance.hpp"

namespace rx
{
  class Device
  {
  public:
    Device( ) = default;
    Device( std::vector<const char*>& extensions, bool initialize = true );
    
    // Creates the logical device and enables all device extensions provided if they are supported.
    // @param queueFamilies - All queue families whose queues should be created.
    // @param extensions - All device extensions that should be enabled.
    void init( std::vector<const char*>& extensions );

  private:
    void checkExtensionSupport( const std::vector<const char*>& extensions ) const;

    vk::UniqueDevice m_device;
  };
}

#endif // DEVICE_HPP