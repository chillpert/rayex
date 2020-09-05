#ifndef DESCRIPTOR_SET_LAYOUT_HPP
#define DESCRIPTOR_SET_LAYOUT_HPP

#include "pch/stdafx.hpp"

namespace rx
{
  /// A wrapper class for a Vulkan descriptor set layout.
  /// @ingroup API
  class DescriptorSetLayout
  {
  public:
    DescriptorSetLayout( ) = default;

    /// @param bindings A vector of Vulkan descriptor set layout bindings.
    /// @param initialize If true, the descriptor set layout will be initialized right away without an additional call to init().
    DescriptorSetLayout( const std::vector<vk::DescriptorSetLayoutBinding> bindings, bool initialize = true );

    /// Calls destroy().
    ~DescriptorSetLayout( );

    /// @return Returns the Vulkan descriptor set layout object.
    inline const vk::DescriptorSetLayout get( ) const { return m_layout; }

    /// @return Returns a vector of all Vulkan descriptor set layout bindings.
    inline const std::vector<vk::DescriptorSetLayoutBinding>& getBindings( ) const { return m_bindings; }

    /// Used to add a single Vulkan descriptor set layout binding.
    /// @param binding The binding to add.
    void addBinding( const vk::DescriptorSetLayoutBinding& binding );

    /// Used to add multiple Vulkan descriptor set layout bindings.
    /// @param bindings A vector of bindings to add.
    void setBindings( const std::vector<vk::DescriptorSetLayoutBinding>& bindings );
    
    /// Removes all existing bindings.
    void clearBindings( );

    /// Creates the descriptor set layout.
    void init( );
    
    /// Creates the descriptor set layout with the provided bindings.
    /// @param bindings A vector of bindings to add.
    void init( const std::vector<vk::DescriptorSetLayoutBinding> bindings );

    /// Destroys the descriptor set layout and removes all bindings.
    void destroy( );

  private:
    vk::DescriptorSetLayout m_layout; ///< The Vulkan descriptor set layout.
    std::vector<vk::DescriptorSetLayoutBinding> m_bindings; ///< A vector containing all Vulkan descriptor set layout bindings for this descriptor set layout.
  };
}

#endif // DESCRIPTOR_SET_LAYOUT_HPP