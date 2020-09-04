#ifndef QUEUES_HPP
#define QUEUES_HPP

#include "stdafx.hpp"

namespace rx
{
  /// Manages the Vulkan queues.
  class Queues
  {
  public:
    /// Retrieves the queue family indices for both graphics and transfer queue families.
    /// @note The function tries to find a transfer queue family index that is not the same as the graphics queue family index.
    /// @see hasDedicatedTransferQueueFamily(vk::PhysicalDevice)
    void init( );

    /// Retrieves a queue handle from each graphics and transfer family indices.
    void retrieveHandles( );

    /// Checks if a given physical device supports the queue capabilities required by the application.
    /// @param physicalDevice The physical device to check.
    static bool isComplete( vk::PhysicalDevice physicalDevice );

    /// Checks if a given physical device has a dedicated transfer queue family index that is not a graphics queue family index.
    /// @param physicalDevice The physical device to check.
    /// @return Returns true if the physical device has a dedicated transfer queue family index.
    static bool hasDedicatedTransferQueueFamily( vk::PhysicalDevice physicalDevice );

  private:
    std::optional<uint32_t> m_graphicsFamilyIndex; ///< The graphics queue family index.
    std::optional<uint32_t> m_transferFamilyIndex; ///< The transfer queue family index.
  };
}

#endif // QUEUES_HPP