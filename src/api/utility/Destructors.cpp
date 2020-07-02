#include "Destructors.hpp"
#include "Components.hpp"

using namespace rx;

namespace vk
{
  namespace Destructor
  {
    void destroyImageView( ImageView imageView )
    {
      if ( imageView )
        g_device.destroyImageView( imageView );
    }

    void destroyImageViews( std::vector<ImageView>& imageViews )
    {
      for ( ImageView& imageView : imageViews )
        destroyImageView( imageView );
    }

    void destroyDescriptorPool( DescriptorPool descriptorPool )
    {
      if ( descriptorPool )
        g_device.destroyDescriptorPool( descriptorPool );
    }

    void destroyCommandPool( CommandPool commandPool )
    {
      if ( commandPool )
        g_device.destroyCommandPool( commandPool );
    }

    void destroyFramebuffer( Framebuffer framebuffer )
    {
      if ( framebuffer )
        g_device.destroy( framebuffer );
    }

    void destroyFramebuffers( std::vector<Framebuffer>& framebuffers )
    {
      for ( Framebuffer& framebuffer : framebuffers )
        destroyFramebuffer( framebuffer );
    }

    void destroyQueryPool( QueryPool queryPool )
    {
      if ( queryPool ) 
        g_device.destroyQueryPool( queryPool );
    }

    void destroyShaderModule( ShaderModule shaderModule )
    {
      if ( shaderModule )
        g_device.destroyShaderModule( shaderModule );
    }

    void freeMemory( DeviceMemory memory )
    {
      if ( memory )
        g_device.freeMemory( memory );
    }
  }
}