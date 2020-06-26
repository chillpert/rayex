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

  struct BufferCreateInfo
  {
    // General information
    vk::PhysicalDevice physicalDevice;
    vk::Device device;

    vk::CommandPool commandPool; // Optional, if there is no staging or copying involved.
    vk::Queue queue; // Optional, if there is no staging or copying involved.

    // Buffer
    void* pNextBuffer = nullptr; // Optional
    vk::BufferCreateFlags bufferFlags; // Optional
    vk::DeviceSize size; // The size required for the buffer.
    vk::BufferUsageFlags usage;
    vk::SharingMode sharingMode;
    std::vector<uint32_t> queueFamilyIndices; // Optional, if sharing mode is not concurrent.

    // Memory
    vk::MemoryPropertyFlags memoryProperties;
    void* pNextMemory = nullptr; // Optional
    vk::DeviceSize memoryOffset = 0;
  };
}

#endif // INITIALIZER_INFO_HPP