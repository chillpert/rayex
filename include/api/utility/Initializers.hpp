#pragma once

#include "api/pathtrace/AccelerationStructure.hpp"

namespace vk
{
  /// A collection of functions to initialize or allocate Vulkan resources.
  /// @ingroup API
  namespace Initializer
  {
    /// Creates the acceleration structure and allocates and binds memory for it.
    /// @param asCreateInfo The Vulkan init info for the acceleration structure.
    /// @return Returns an RAYEX_NAMESPACE::AccelerationStructure object that contains the AS itself as well as the memory for it.
    auto initAccelerationStructure( AccelerationStructureCreateInfoKHR& asCreateInfo ) -> RAYEX_NAMESPACE::AccelerationStructure;

    /// Retrieves the most suited GPU on the current machine.
    ///
    /// If a device is found its properties and features will be retrieved.
    /// @return Returns true if the initialization was successful.
    /// @note Multiple GPUs can not be utilized.
    //auto initPhysicalDevice( ) -> PhysicalDevice;

    /// Retrieves the queue family indices for both graphics and transfer queue families.
    /// @note The function tries to find a transfer queue family index that is not the same as the graphics queue family index.
    /// @see hasDedicatedTransferQueueFamily(vk::PhysicalDevice)
    void initQueueFamilyIndices( );

    /// Creates the logical device and enables all device extensions provided if they are supported.
    /// @param extensions All device extensions that should be enabled.
    /// @return Returns true if the initilization was successful.
    auto initDevice( std::vector<const char*>& extensions ) -> UniqueDevice;

    /// Initializes the instance object and checks support for any given layer and extension.
    /// @param layers All validation layers that should be activated.
    /// @param extensions All instance extensions that should be activated.
    /// @note The given vector with the extensions will be appended by ones that are required by the window.
    /// @return Returns true if initialization was successful.
    auto initInstance( const std::vector<const char*>& layers, std::vector<const char*>& extensions ) -> UniqueInstance;
  } // namespace Initializer
} // namespace vk
