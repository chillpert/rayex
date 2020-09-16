#include "api/descriptors/DescriptorSet.hpp"
#include "api/Components.hpp"

namespace RENDERER_NAMESPACE
{
  DescriptorSetLayout::DescriptorSetLayout( const std::vector<vk::DescriptorSetLayoutBinding> bindings, bool initialize )
  {
    if ( initialize )
      init( bindings );
  }

  DescriptorSetLayout::~DescriptorSetLayout( )
  {
    if ( m_layout )
      destroy( );
  }

  void DescriptorSetLayout::addBinding( const vk::DescriptorSetLayoutBinding& binding )
  {
    if ( m_layout )
      RX_ERROR( "Failed to add binding because the descriptor set layout was already initialized." );

    m_bindings.push_back( binding );
  }

  void DescriptorSetLayout::setBindings( const std::vector<vk::DescriptorSetLayoutBinding>& bindings )
  {
    if ( m_layout )
      RX_ERROR( "Failed to add binding because the descriptor set layout was already initialized." );

    m_bindings = bindings;
  }

  void DescriptorSetLayout::clearBindings( )
  {
    m_bindings.clear( );
  }

  void DescriptorSetLayout::init( )
  {
    vk::DescriptorSetLayoutCreateInfo createInfo( { },                                            // flags
                                                  static_cast< uint32_t >( m_bindings.size( ) ) , // bindingCount
                                                  m_bindings.data( ) );                           // pBindings
 
    m_layout = g_device.createDescriptorSetLayout( createInfo );
    if ( !m_layout )
      RX_ERROR( "Failed to create descriptor set layout." );
  }

  void DescriptorSetLayout::init( const std::vector<vk::DescriptorSetLayoutBinding> bindings )
  {
    setBindings( bindings );

    init( );
  }

  void DescriptorSetLayout::destroy( )
  {
    if ( m_layout )
    {
      g_device.destroyDescriptorSetLayout( m_layout );
      m_layout = nullptr;
    }

    m_bindings.clear( );
  }
}