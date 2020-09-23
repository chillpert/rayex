#include "api/utility/Initializers.hpp"
#include "api/utility/Helpers.hpp"
#include "api/buffers/CommandBuffer.hpp"
#include "api/misc/Vertex.hpp"

namespace vk
{
  namespace Initializer
  {
    UniqueFence initFenceUnique( FenceCreateFlags flags )
    {
      FenceCreateInfo createInfo( flags );

      UniqueFence fence = RENDERER_NAMESPACE::g_device.createFenceUnique( createInfo );
      RX_ASSERT( fence, "Failed to create fence." );

      return fence;
    }

    Fence initFence( FenceCreateFlags flags )
    {
      FenceCreateInfo createInfo( flags );

      Fence fence = RENDERER_NAMESPACE::g_device.createFence( createInfo );
      RX_ASSERT( fence, "Failed to create fence." );

      return fence;
    }

    UniqueSemaphore initSemaphoreUnique( SemaphoreCreateFlags flags )
    {
      SemaphoreCreateInfo createInfo( flags ); 

      UniqueSemaphore semaphore = RENDERER_NAMESPACE::g_device.createSemaphoreUnique( createInfo );
      RX_ASSERT( semaphore, "Failed to create semaphore." );

      return semaphore;
    }

    Semaphore initSemaphore( SemaphoreCreateFlags flags )
    {
      SemaphoreCreateInfo createInfo( flags );

      Semaphore semaphore = RENDERER_NAMESPACE::g_device.createSemaphore( createInfo );
      RX_ASSERT( semaphore, "Failed to create semaphore." );

      return semaphore;
    }

    UniqueCommandPool initCommandPoolUnique( uint32_t queueFamilyIndex, CommandPoolCreateFlags flags )
    {
      CommandPoolCreateInfo createInfo( flags, queueFamilyIndex );

      UniqueCommandPool commandPool = RENDERER_NAMESPACE::g_device.createCommandPoolUnique( createInfo );
      RX_ASSERT( commandPool, "Failed to create command pool." );

      return commandPool;
    }

    CommandPool initCommandPool( uint32_t queueFamilyIndex, CommandPoolCreateFlags flags )
    {
      CommandPoolCreateInfo createInfo( flags, queueFamilyIndex );

      CommandPool commandPool = RENDERER_NAMESPACE::g_device.createCommandPool( createInfo );
      RX_ASSERT( commandPool, "Failed to create command pool." );

      return commandPool;
    }

    UniqueDescriptorPool initDescriptorPoolUnique( const std::vector<DescriptorPoolSize>& poolSizes, uint32_t maxSets, DescriptorPoolCreateFlags flags )
    {
      DescriptorPoolCreateInfo createInfo( flags,                                   // flags
                                           maxSets,                                 // maxSets
                                           static_cast<uint32_t>(poolSizes.size()), // poolSizeCount
                                           poolSizes.data() );                      // pPoolSizes

      UniqueDescriptorPool descriptorPool = RENDERER_NAMESPACE::g_device.createDescriptorPoolUnique( createInfo );
      RX_ASSERT( descriptorPool, "Failed to create descriptor pool." );

      return descriptorPool;
    }

    DescriptorPool initDescriptorPool( const std::vector<DescriptorPoolSize>& poolSizes, uint32_t maxSets, DescriptorPoolCreateFlags flags )
    {
      DescriptorPoolCreateInfo createInfo( flags,                                   // flags
                                           maxSets,                                 // maxSets
                                           static_cast< uint32_t >( poolSizes.size( ) ), // poolSizeCount
                                           poolSizes.data( ) );                      // pPoolSizes

      DescriptorPool descriptorPool = RENDERER_NAMESPACE::g_device.createDescriptorPool( createInfo );
      RX_ASSERT( descriptorPool, "Failed to create descriptor pool." );

      return descriptorPool;
    }

    UniqueDeviceMemory allocateMemoryUnique( Image image, MemoryPropertyFlags propertyFlags, void* pNext )
    {
      auto memoryRequirements = RENDERER_NAMESPACE::g_device.getImageMemoryRequirements( image );

      vk::MemoryAllocateInfo allocateInfo( memoryRequirements.size,                                                                             // allocationSize 
                                           Helper::findMemoryType( RENDERER_NAMESPACE::g_physicalDevice, memoryRequirements.memoryTypeBits, propertyFlags ) );  // memoryTypeIndex

      allocateInfo.pNext = pNext;

      UniqueDeviceMemory memory = RENDERER_NAMESPACE::g_device.allocateMemoryUnique( allocateInfo );
      RX_ASSERT( memory, "Failed to create memory for image." );

      RENDERER_NAMESPACE::g_device.bindImageMemory( image, memory.get( ), 0 );

      return memory;
    }

    DeviceMemory allocateMemory( Image image, MemoryPropertyFlags propertyFlags, void* pNext )
    {
      auto memoryRequirements = RENDERER_NAMESPACE::g_device.getImageMemoryRequirements( image );

      vk::MemoryAllocateInfo allocateInfo( memoryRequirements.size,                                                                             // allocationSize 
                                           Helper::findMemoryType( RENDERER_NAMESPACE::g_physicalDevice, memoryRequirements.memoryTypeBits, propertyFlags ) );  // memoryTypeIndex

      allocateInfo.pNext = pNext;

      DeviceMemory memory = RENDERER_NAMESPACE::g_device.allocateMemory( allocateInfo );
      RX_ASSERT( memory, "Failed to create memory for image." );

      RENDERER_NAMESPACE::g_device.bindImageMemory( image, memory, 0 );

      return memory;
    }

    UniqueDeviceMemory allocateMemoryUnique( Buffer buffer, MemoryPropertyFlags propertyFlags, void* pNext )
    {
      auto memoryRequirements = RENDERER_NAMESPACE::g_device.getBufferMemoryRequirements( buffer );

      vk::MemoryAllocateInfo allocateInfo( memoryRequirements.size,                                                                             // allocationSize 
                                           Helper::findMemoryType( RENDERER_NAMESPACE::g_physicalDevice, memoryRequirements.memoryTypeBits, propertyFlags ) );  // memoryTypeIndex

      allocateInfo.pNext = pNext;

      UniqueDeviceMemory memory = RENDERER_NAMESPACE::g_device.allocateMemoryUnique( allocateInfo );
      RX_ASSERT( memory, "Failed to create memory for image." );

      RENDERER_NAMESPACE::g_device.bindBufferMemory( buffer, memory.get( ), 0 );

      return memory;;
    }

    void allocateMemory( RENDERER_NAMESPACE::AccelerationStructure& as )
    {
      AccelerationStructureMemoryRequirementsInfoKHR memInfo( AccelerationStructureMemoryRequirementsTypeKHR::eObject, // type
                                                              AccelerationStructureBuildTypeKHR::eDevice,              // buildType
                                                              as.as );                                                 // accelerationStructure

      MemoryRequirements2 memoryRequirements = RENDERER_NAMESPACE::g_device.getAccelerationStructureMemoryRequirementsKHR( memInfo );
      
      MemoryAllocateFlagsInfo allocateFlags( MemoryAllocateFlagBits::eDeviceAddress, // flags
                                                 { } );                              // deviceMask

      MemoryAllocateInfo allocateInfo( memoryRequirements.memoryRequirements.size,                                                                                                   // allocationSize
                                       Helper::findMemoryType( RENDERER_NAMESPACE::g_physicalDevice, memoryRequirements.memoryRequirements.memoryTypeBits, MemoryPropertyFlagBits::eDeviceLocal ) ); // memoryTypeIndex

      as.memory = RENDERER_NAMESPACE::g_device.allocateMemory( allocateInfo );
      RX_ASSERT( as.memory, "Failed to create memory for acceleration structure." );

      BindAccelerationStructureMemoryInfoKHR bindInfo( as.as,     // accelerationStructure
                                                       as.memory, // memory
                                                       0,         // memoryOffset
                                                       0,         // deviceIndexCount
                                                       nullptr ); // pDeviceIndices

      if ( RENDERER_NAMESPACE::g_device.bindAccelerationStructureMemoryKHR( 1, &bindInfo ) != Result::eSuccess )
        RX_ERROR( "Failed to bind acceleration structure memory." );
    }

    DeviceMemory allocateMemory( Buffer buffer, MemoryPropertyFlags propertyFlags, void* pNext )
    {
      auto memoryRequirements = RENDERER_NAMESPACE::g_device.getBufferMemoryRequirements( buffer );

      MemoryAllocateInfo allocateInfo( memoryRequirements.size,                                                                             // allocationSize 
                                       Helper::findMemoryType( RENDERER_NAMESPACE::g_physicalDevice, memoryRequirements.memoryTypeBits, propertyFlags ) );  // memoryTypeIndex

      allocateInfo.pNext = pNext;

      DeviceMemory memory = RENDERER_NAMESPACE::g_device.allocateMemory( allocateInfo );
      RX_ASSERT( memory, "Failed to create memory for image." );

      RENDERER_NAMESPACE::g_device.bindBufferMemory( buffer, memory, 0 );

      return memory;;
    }

    UniqueImageView initImageViewUnique( Image image, Format format, ImageAspectFlags aspectFlags )
    {
      ComponentMapping components =
      {
        ComponentSwizzle::eIdentity,
        ComponentSwizzle::eIdentity,
        ComponentSwizzle::eIdentity,
        ComponentSwizzle::eIdentity
      };

      ImageSubresourceRange subresourceRange =
      {
        aspectFlags,  // aspectMask
        0u,           // baseMipLevel
        1u,           // levelCount
        0u,           // baseArrayLayer
        1u            // layerCount
      };

      ImageViewCreateInfo createInfo( { },                // flags
                                      image,              // image
                                      ImageViewType::e2D, // viewType
                                      format,             // format
                                      components,         // components
                                      subresourceRange ); // subresourceRange
                            
      UniqueImageView imageView = RENDERER_NAMESPACE::g_device.createImageViewUnique( createInfo );
      RX_ASSERT( imageView, "Failed to create image view." );

      return imageView;
    }

    ImageView initImageView( Image image, Format format, ImageAspectFlags aspectFlags )
    {
      ComponentMapping components =
      {
        vk::ComponentSwizzle::eIdentity,
        vk::ComponentSwizzle::eIdentity,
        vk::ComponentSwizzle::eIdentity,
        vk::ComponentSwizzle::eIdentity
      };

      ImageSubresourceRange subresourceRange =
      {
        aspectFlags,  // aspectMask
        0u,           // baseMipLevel
        1u,           // levelCount
        0u,           // baseArrayLayer
        1u            // layerCount
      };

      ImageViewCreateInfo createInfo( { },                // flags
                                      image,              // image
                                      ImageViewType::e2D, // viewType
                                      format,             // format
                                      components,         // components
                                      subresourceRange ); // subresourceRange

      ImageView imageView = RENDERER_NAMESPACE::g_device.createImageView( createInfo );
      RX_ASSERT( imageView, "Failed to create image view." );

      return imageView;
    }

    UniqueSampler initSamplerUnique( const SamplerCreateInfo& createInfo )
    {
      UniqueSampler sampler = RENDERER_NAMESPACE::g_device.createSamplerUnique( createInfo );
      RX_ASSERT( sampler, "Failed to create sampler." );

      return sampler;
    }

    Sampler initSampler( const SamplerCreateInfo& createInfo )
    {
      Sampler sampler = RENDERER_NAMESPACE::g_device.createSampler( createInfo );
      RX_ASSERT( sampler, "Failed to create sampler." );

      return sampler;
    }

    UniqueFramebuffer initFramebufferUnique( const std::vector<ImageView>& attachments, RenderPass renderPass, const Extent2D& extent )
    {
      FramebufferCreateInfo createInfo( { },                                            // flags
                                        renderPass,                                     // renderPass
                                        static_cast< uint32_t >( attachments.size( ) ), // attachmentCount
                                        attachments.data( ),                            // pAttachments
                                        extent.width,                                   // width
                                        extent.height,                                  // height
                                        1u );                                           // layers

      UniqueFramebuffer framebuffer = RENDERER_NAMESPACE::g_device.createFramebufferUnique( createInfo );
      RX_ASSERT( framebuffer, "Failed to create framebuffer." );

      return framebuffer;
    }

    Framebuffer initFramebuffer( const std::vector<ImageView>& attachments, RenderPass renderPass, const Extent2D& extent )
    {
      FramebufferCreateInfo createInfo( { },                                        // flags
                                        renderPass,                                 // renderPass
                                        static_cast<uint32_t>(attachments.size()),  // attachmentCount
                                        attachments.data(),                         // pAttachments
                                        extent.width,                               // width
                                        extent.height,                              // height
                                        1u );                                       // layers

      Framebuffer framebuffer = RENDERER_NAMESPACE::g_device.createFramebuffer( createInfo );
      RX_ASSERT( framebuffer, "Failed to create framebuffer." );

      return framebuffer;
    }

    UniqueQueryPool initQueryPoolUnique( uint32_t count, QueryType type )
    {
      QueryPoolCreateInfo createInfo( { },    // flags
                                      type,   // queryType
                                      count,  // queryCount
                                      { } );  // pipelineStatistics
      
      UniqueQueryPool queryPool = RENDERER_NAMESPACE::g_device.createQueryPoolUnique( createInfo );
      RX_ASSERT( queryPool, "Failed to create query pool." );

      return queryPool;
    }

    QueryPool initQueryPool( uint32_t count, QueryType type )
    {
      QueryPoolCreateInfo createInfo( { },    // flags
                                      type,   // queryType
                                      count,  // queryCount
                                      { } );  // pipelineStatistics

      QueryPool queryPool = RENDERER_NAMESPACE::g_device.createQueryPool( createInfo );
      RX_ASSERT( queryPool, "Failed to create query pool." );

      return queryPool;
    }

    UniqueShaderModule initShaderModuleUnique( const std::string& path )
    {
      std::vector<char> source = RENDERER_NAMESPACE::util::parseShader( path );

      vk::ShaderModuleCreateInfo createInfo( { },                                                     // flags
                                             source.size( ),                                          // codeSize
                                             reinterpret_cast< const uint32_t* >( source.data( ) ) ); // pCode

      UniqueShaderModule shaderModule = RENDERER_NAMESPACE::g_device.createShaderModuleUnique( createInfo );
      RX_ASSERT( shaderModule, "Failed to create shader module." );

      return shaderModule;
    }

    ShaderModule initShaderModule( const std::string& path )
    {
      std::vector<char> source = RENDERER_NAMESPACE::util::parseShader( path );

      vk::ShaderModuleCreateInfo createInfo( { },                                                     // flags
                                             source.size( ),                                          // codeSize
                                             reinterpret_cast< const uint32_t* >( source.data( ) ) ); // pCode

      ShaderModule shaderModule = RENDERER_NAMESPACE::g_device.createShaderModule( createInfo );
      RX_ASSERT( shaderModule, "Failed to create shader module." );

      return shaderModule;
    }

    RENDERER_NAMESPACE::AccelerationStructure initAccelerationStructure( const vk::AccelerationStructureCreateInfoKHR& asCreateInfo )
    {
      RENDERER_NAMESPACE::AccelerationStructure resultAs;
      resultAs.as = RENDERER_NAMESPACE::g_device.createAccelerationStructureKHR( asCreateInfo, nullptr );

      allocateMemory( resultAs );

      return resultAs;
    }

    bool initPhysicalDevice( PhysicalDevice physicalDevice )
    {
      auto physicalDevices = RENDERER_NAMESPACE::g_instance.enumeratePhysicalDevices( );

      std::vector<std::pair<unsigned int, std::string>> results;

      unsigned int score = 0;
      for ( const auto& it : physicalDevices )
      {
        auto temp = Helper::evaluatePhysicalDevice( it );
        results.push_back( temp );

        if ( temp.first > score )
        {
          physicalDevice = it;
          score = temp.first;
        }
      }

      if ( !physicalDevice )
      {
        RX_ERROR( "No suitable device was found." );
        return false;
      }

      const std::string separator = "===================================================================";
      std::cout << "Physical device report: " << "\n" << separator << "\n" << "Device name" << "\t\t\t" << "Score" << std::endl << separator << "\n";

      for ( const auto& result : results )
        std::cout << std::left << std::setw( 32 ) << std::setfill( ' ' ) << result.second << std::left << std::setw( 32 ) << std::setfill( ' ' ) << result.first << std::endl;

      // Print information about the GPU that was selected.
      auto properties = physicalDevice.getProperties( );
      RX_SUCCESS( "Selected GPU: ", properties.deviceName );

      RENDERER_NAMESPACE::g_physicalDeviceLimits = properties.limits;
      RENDERER_NAMESPACE::g_physicalDevice = physicalDevice;

      return true;
    }

    bool initQueueFamilyIndices( )
    {
      std::optional<uint32_t> graphicsFamilyIndex;
      std::optional<uint32_t> transferFamilyIndex;

      auto queueFamilyProperties = RENDERER_NAMESPACE::g_physicalDevice.getQueueFamilyProperties( );
      std::vector<uint32_t> queueFamilies( queueFamilyProperties.size( ) );

      bool dedicatedTransferQueueFamily = Helper::isPhysicalDeviceWithDedicatedTransferQueueFamily( RENDERER_NAMESPACE::g_physicalDevice );

      for ( uint32_t index = 0; index < static_cast<uint32_t>( queueFamilies.size( ) ); ++index )
      {
        if ( queueFamilyProperties[index].queueFlags & vk::QueueFlagBits::eGraphics && !graphicsFamilyIndex.has_value( ) )
        {
          if ( RENDERER_NAMESPACE::g_physicalDevice.getSurfaceSupportKHR( index, RENDERER_NAMESPACE::g_surface ) )
            graphicsFamilyIndex = index;
        }

        if ( dedicatedTransferQueueFamily )
        {
          if ( !( queueFamilyProperties[index].queueFlags & QueueFlagBits::eGraphics ) && !transferFamilyIndex.has_value( ) )
            transferFamilyIndex = index;
        }
        else
        {
          if ( queueFamilyProperties[index].queueFlags & QueueFlagBits::eTransfer && !transferFamilyIndex.has_value( ) )
            transferFamilyIndex = index;
        }
      }

      if ( !graphicsFamilyIndex.has_value( ) || !transferFamilyIndex.has_value( ) )
      {
        RX_ERROR( "Failed to retrieve queue family indices." );
        return false;
      }

      RENDERER_NAMESPACE::g_graphicsFamilyIndex = graphicsFamilyIndex.value( );
      RENDERER_NAMESPACE::g_transferFamilyIndex = transferFamilyIndex.value( );
    
      return true;
    }
  }
}
