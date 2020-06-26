#ifndef INITIALIZER_INFO_HPP
#define INITIALIZER_INFO_HPP

#include "WindowBase.hpp"

namespace RX
{
  struct SurfaceInfo
  {
    WindowBase* window;
    vk::Instance instance;
    vk::Format format = vk::Format::eB8G8R8A8Unorm;
    vk::ColorSpaceKHR colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
    vk::PresentModeKHR presentMode = vk::PresentModeKHR::eMailbox;
  }; 
}

#endif // INITIALIZER_INFO_HPP