#include "api/descriptors/DescriptorSetLayout.hpp"
#include "api/misc/Components.hpp"

namespace RAYEXEC_NAMESPACE
{
  DescriptorSetLayout::DescriptorSetLayout( const std::vector<vk::DescriptorSetLayoutBinding> bindings, bool initialize )
  {
    if ( initialize )
      init( bindings );
  }

  DescriptorSetLayout::~DescriptorSetLayout( )
  {
    if ( this->layout )
      destroy( );
  }

  void DescriptorSetLayout::addBinding( const vk::DescriptorSetLayoutBinding& binding )
  {
    if ( this->layout )
      RX_ERROR( "Failed to add binding because the descriptor set layout was already initialized." );

    this->bindings.push_back( binding );
  }

  void DescriptorSetLayout::setBindings( const std::vector<vk::DescriptorSetLayoutBinding>& bindings )
  {
    if ( this->layout )
      RX_ERROR( "Failed to add binding because the descriptor set layout was already initialized." );

    this->bindings = bindings;
  }

  void DescriptorSetLayout::clearBindings( )
  {
    this->bindings.clear( );
  }

  void DescriptorSetLayout::init( )
  {
    vk::DescriptorSetLayoutCreateInfo createInfo( { },                                            // flags
                                                  static_cast< uint32_t >( this->bindings.size( ) ) , // bindingCount
                                                  this->bindings.data( ) );                           // pBindings
 
    this->layout = g_device.createDescriptorSetLayout( createInfo );
    if ( !this->layout )
      RX_ERROR( "Failed to create descriptor set layout." );
  }

  void DescriptorSetLayout::init( const std::vector<vk::DescriptorSetLayoutBinding> bindings )
  {
    setBindings( bindings );

    init( );
  }

  void DescriptorSetLayout::destroy( )
  {
    if ( this->layout )
    {
      g_device.destroyDescriptorSetLayout( this->layout );
      this->layout = nullptr;
    }

    this->bindings.clear( );
  }
}