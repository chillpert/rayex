#include "api/utility/Initializers.hpp"
#include "api/utility/Helpers.hpp"
#include "api/buffers/CommandBuffer.hpp"

namespace vk
{
  namespace Initializer
  {
    UniqueFence initFenceUnique( FenceCreateFlags flags )
    {
      FenceCreateInfo createInfo( flags );

      UniqueFence fence = rx::g_device.createFenceUnique( createInfo );
      RX_ASSERT( fence, "Failed to create fence." );

      return fence;
    }

    Fence initFence( FenceCreateFlags flags )
    {
      FenceCreateInfo createInfo( flags );

      Fence fence = rx::g_device.createFence( createInfo );
      RX_ASSERT( fence, "Failed to create fence." );

      return fence;
    }

    UniqueSemaphore initSemaphoreUnique( SemaphoreCreateFlags flags )
    {
      SemaphoreCreateInfo createInfo( flags ); 

      UniqueSemaphore semaphore = rx::g_device.createSemaphoreUnique( createInfo );
      RX_ASSERT( semaphore, "Failed to create semaphore." );

      return semaphore;
    }

    Semaphore initSemaphore( SemaphoreCreateFlags flags )
    {
      SemaphoreCreateInfo createInfo( flags );

      Semaphore semaphore = rx::g_device.createSemaphore( createInfo );
      RX_ASSERT( semaphore, "Failed to create semaphore." );

      return semaphore;
    }

    UniqueCommandPool initCommandPoolUnique( uint32_t queueFamilyIndex, CommandPoolCreateFlags flags )
    {
      CommandPoolCreateInfo createInfo( flags, queueFamilyIndex );

      UniqueCommandPool commandPool = rx::g_device.createCommandPoolUnique( createInfo );
      RX_ASSERT( commandPool, "Failed to create command pool." );

      return commandPool;
    }

    CommandPool initCommandPool( uint32_t queueFamilyIndex, CommandPoolCreateFlags flags )
    {
      CommandPoolCreateInfo createInfo( flags, queueFamilyIndex );

      CommandPool commandPool = rx::g_device.createCommandPool( createInfo );
      RX_ASSERT( commandPool, "Failed to create command pool." );

      return commandPool;
    }

    UniqueDescriptorPool initDescriptorPoolUnique( const std::vector<DescriptorPoolSize>& poolSizes, uint32_t maxSets, DescriptorPoolCreateFlags flags )
    {
      DescriptorPoolCreateInfo createInfo( flags,                                   // flags
                                           maxSets,                                 // maxSets
                                           static_cast<uint32_t>(poolSizes.size()), // poolSizeCount
                                           poolSizes.data() );                      // pPoolSizes

      UniqueDescriptorPool descriptorPool = rx::g_device.createDescriptorPoolUnique( createInfo );
      RX_ASSERT( descriptorPool, "Failed to create descriptor pool." );

      return descriptorPool;
    }

    DescriptorPool initDescriptorPool( const std::vector<DescriptorPoolSize>& poolSizes, uint32_t maxSets, DescriptorPoolCreateFlags flags )
    {
      DescriptorPoolCreateInfo createInfo( flags,                                   // flags
                                           maxSets,                                 // maxSets
                                           static_cast< uint32_t >( poolSizes.size( ) ), // poolSizeCount
                                           poolSizes.data( ) );                      // pPoolSizes

      DescriptorPool descriptorPool = rx::g_device.createDescriptorPool( createInfo );
      RX_ASSERT( descriptorPool, "Failed to create descriptor pool." );

      return descriptorPool;
    }

    UniqueDeviceMemory allocateMemoryUnique( Image image, MemoryPropertyFlags propertyFlags, void* pNext )
    {
      auto memoryRequirements = rx::g_device.getImageMemoryRequirements( image );

      vk::MemoryAllocateInfo allocateInfo( memoryRequirements.size,                                                                             // allocationSize 
                                           Helper::findMemoryType( rx::g_physicalDevice, memoryRequirements.memoryTypeBits, propertyFlags ) );  // memoryTypeIndex

      allocateInfo.pNext = pNext;

      UniqueDeviceMemory memory = rx::g_device.allocateMemoryUnique( allocateInfo );
      RX_ASSERT( memory, "Failed to create memory for image." );

      rx::g_device.bindImageMemory( image, memory.get( ), 0 );

      return memory;
    }

    DeviceMemory allocateMemory( Image image, MemoryPropertyFlags propertyFlags, void* pNext )
    {
      auto memoryRequirements = rx::g_device.getImageMemoryRequirements( image );

      vk::MemoryAllocateInfo allocateInfo( memoryRequirements.size,                                                                             // allocationSize 
                                           Helper::findMemoryType( rx::g_physicalDevice, memoryRequirements.memoryTypeBits, propertyFlags ) );  // memoryTypeIndex

      allocateInfo.pNext = pNext;

      DeviceMemory memory = rx::g_device.allocateMemory( allocateInfo );
      RX_ASSERT( memory, "Failed to create memory for image." );

      rx::g_device.bindImageMemory( image, memory, 0 );

      return memory;
    }

    UniqueDeviceMemory allocateMemoryUnique( Buffer buffer, MemoryPropertyFlags propertyFlags, void* pNext )
    {
      auto memoryRequirements = rx::g_device.getBufferMemoryRequirements( buffer );

      vk::MemoryAllocateInfo allocateInfo( memoryRequirements.size,                                                                             // allocationSize 
                                           Helper::findMemoryType( rx::g_physicalDevice, memoryRequirements.memoryTypeBits, propertyFlags ) );  // memoryTypeIndex

      allocateInfo.pNext = pNext;

      UniqueDeviceMemory memory = rx::g_device.allocateMemoryUnique( allocateInfo );
      RX_ASSERT( memory, "Failed to create memory for image." );

      rx::g_device.bindBufferMemory( buffer, memory.get( ), 0 );

      return memory;;
    }

    void allocateMemory( rx::AccelerationStructure& as )
    {
      AccelerationStructureMemoryRequirementsInfoKHR memInfo( AccelerationStructureMemoryRequirementsTypeKHR::eObject, // type
                                                              AccelerationStructureBuildTypeKHR::eDevice,              // buildType
                                                              as.as );                                                 // accelerationStructure

      MemoryRequirements2 memoryRequirements = rx::g_device.getAccelerationStructureMemoryRequirementsKHR( memInfo );
      
      MemoryAllocateFlagsInfo allocateFlags( MemoryAllocateFlagBits::eDeviceAddress, // flags
                                                 { } );                              // deviceMask

      MemoryAllocateInfo allocateInfo( memoryRequirements.memoryRequirements.size,                                                                                                   // allocationSize
                                       Helper::findMemoryType( rx::g_physicalDevice, memoryRequirements.memoryRequirements.memoryTypeBits, MemoryPropertyFlagBits::eDeviceLocal ) ); // memoryTypeIndex

      as.memory = rx::g_device.allocateMemory( allocateInfo );
      RX_ASSERT( as.memory, "Failed to create memory for acceleration structure." );

      BindAccelerationStructureMemoryInfoKHR bindInfo( as.as,     // accelerationStructure
                                                       as.memory, // memory
                                                       0,         // memoryOffset
                                                       0,         // deviceIndexCount
                                                       nullptr ); // pDeviceIndices

      if ( rx::g_device.bindAccelerationStructureMemoryKHR( 1, &bindInfo ) != Result::eSuccess )
        RX_ERROR( "Failed to bind acceleration structure memory." );
    }

    DeviceMemory allocateMemory( Buffer buffer, MemoryPropertyFlags propertyFlags, void* pNext )
    {
      auto memoryRequirements = rx::g_device.getBufferMemoryRequirements( buffer );

      MemoryAllocateInfo allocateInfo( memoryRequirements.size,                                                                             // allocationSize 
                                       Helper::findMemoryType( rx::g_physicalDevice, memoryRequirements.memoryTypeBits, propertyFlags ) );  // memoryTypeIndex

      allocateInfo.pNext = pNext;

      DeviceMemory memory = rx::g_device.allocateMemory( allocateInfo );
      RX_ASSERT( memory, "Failed to create memory for image." );

      rx::g_device.bindBufferMemory( buffer, memory, 0 );

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
                            
      UniqueImageView imageView = rx::g_device.createImageViewUnique( createInfo );
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

      ImageView imageView = rx::g_device.createImageView( createInfo );
      RX_ASSERT( imageView, "Failed to create image view." );

      return imageView;
    }

    UniqueSampler initSamplerUnique( const SamplerCreateInfo& createInfo )
    {
      UniqueSampler sampler = rx::g_device.createSamplerUnique( createInfo );
      RX_ASSERT( sampler, "Failed to create sampler." );

      return sampler;
    }

    Sampler initSampler( const SamplerCreateInfo& createInfo )
    {
      Sampler sampler = rx::g_device.createSampler( createInfo );
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

      UniqueFramebuffer framebuffer = rx::g_device.createFramebufferUnique( createInfo );
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

      Framebuffer framebuffer = rx::g_device.createFramebuffer( createInfo );
      RX_ASSERT( framebuffer, "Failed to create framebuffer." );

      return framebuffer;
    }

    UniqueQueryPool initQueryPoolUnique( uint32_t count, QueryType type )
    {
      QueryPoolCreateInfo createInfo( { },    // flags
                                      type,   // queryType
                                      count,  // queryCount
                                      { } );  // pipelineStatistics
      
      UniqueQueryPool queryPool = rx::g_device.createQueryPoolUnique( createInfo );
      RX_ASSERT( queryPool, "Failed to create query pool." );

      return queryPool;
    }

    QueryPool initQueryPool( uint32_t count, QueryType type )
    {
      QueryPoolCreateInfo createInfo( { },    // flags
                                      type,   // queryType
                                      count,  // queryCount
                                      { } );  // pipelineStatistics

      QueryPool queryPool = rx::g_device.createQueryPool( createInfo );
      RX_ASSERT( queryPool, "Failed to create query pool." );

      return queryPool;
    }

    UniqueShaderModule initShaderModuleUnique( const std::string& path )
    {
      std::vector<char> source = rx::util::parseShader( path );

      vk::ShaderModuleCreateInfo createInfo( { },                                                     // flags
                                             source.size( ),                                          // codeSize
                                             reinterpret_cast< const uint32_t* >( source.data( ) ) ); // pCode

      UniqueShaderModule shaderModule = rx::g_device.createShaderModuleUnique( createInfo );
      RX_ASSERT( shaderModule, "Failed to create shader module." );

      return shaderModule;
    }

    ShaderModule initShaderModule( const std::string& path )
    {
      std::vector<char> source = rx::util::parseShader( path );

      vk::ShaderModuleCreateInfo createInfo( { },                                                     // flags
                                             source.size( ),                                          // codeSize
                                             reinterpret_cast< const uint32_t* >( source.data( ) ) ); // pCode

      ShaderModule shaderModule = rx::g_device.createShaderModule( createInfo );
      RX_ASSERT( shaderModule, "Failed to create shader module." );

      return shaderModule;
    }

    rx::AccelerationStructure initAccelerationStructure( const vk::AccelerationStructureCreateInfoKHR& asCreateInfo )
    {
      rx::AccelerationStructure resultAs;
      resultAs.as = rx::g_device.createAccelerationStructureKHR( asCreateInfo, nullptr );

      allocateMemory( resultAs );

      return resultAs;
    }

    std::pair<PhysicalDevice, bool> initPhysicalDevice( )
    {
      PhysicalDevice physicalDevice;
      bool result = true;
      auto physicalDevices = rx::g_instance.enumeratePhysicalDevices( );

      std::vector<std::pair<unsigned int, std::string>> results;

      unsigned int score = 0;
      for ( const auto& it : physicalDevices )
      {
        auto temp = Helper::evaluate( it );
        results.push_back( temp );

        if ( temp.first > score )
        {
          physicalDevice = it;
          score = temp.first;
        }
      }

      if ( !physicalDevice )
      {
        result = false;
        RX_ERROR( "No suitable device was found." );
      }

      const std::string separator = "===================================================================";
      std::cout << "Physical device report: " << "\n" << separator << "\n" << "Device name" << "\t\t\t" << "Score" << std::endl << separator << "\n";

      for ( const auto& result : results )
        std::cout << std::left << std::setw( 32 ) << std::setfill( ' ' ) << result.second << std::left << std::setw( 32 ) << std::setfill( ' ' ) << result.first << std::endl;

      // Print information about the GPU that was selected.
      auto properties = physicalDevice.getProperties( );
      RX_SUCCESS( "Selected GPU: ", properties.deviceName );

      rx::g_physicalDeviceLimits = properties.limits;
      rx::g_physicalDevice = physicalDevice;

      return { physicalDevice, result };
    }
  }
}
