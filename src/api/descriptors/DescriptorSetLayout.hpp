#ifndef DESCRIPTOR_SET_LAYOUT_HPP
#define DESCRIPTOR_SET_LAYOUT_HPP

#include "pch/stdafx.hpp"

namespace rx
{
  class DescriptorSetLayout
  {
  public:
    ~DescriptorSetLayout( );

    inline const vk::DescriptorSetLayout get( ) const { return m_layout; }
    inline const std::vector<vk::DescriptorSetLayoutBinding>& getBindings( ) const { return m_bindings; }

    void addBinding( const vk::DescriptorSetLayoutBinding& binding );
    void setBindings( const std::vector<vk::DescriptorSetLayoutBinding>& bindings );
    void clearBindings( );

    void init( );
    void init( const std::vector<vk::DescriptorSetLayoutBinding> bindings );
    void destroy( );

  private:
    vk::DescriptorSetLayout m_layout;

    std::vector<vk::DescriptorSetLayoutBinding> m_bindings;
  };
}

#endif // DESCRIPTOR_SET_LAYOUT_HPP