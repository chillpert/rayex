#include "Helpers.hpp"
#include "Components.hpp"
#include "CommandBuffer.hpp"

namespace vk
{
  namespace Helper
  {
    PipelineShaderStageCreateInfo getPipelineShaderStageCreateInfo( ShaderStageFlagBits stage, ShaderModule module, const char* name, SpecializationInfo* specializationInfo )
    {
      PipelineShaderStageCreateInfo createInfo( { },                  // flags
                                                stage,                // stage
                                                module,               // module
                                                "main",               // pName
                                                specializationInfo ); // pSpecializationInfo

      return createInfo;
    }

    ImageCreateInfo getImageCreateInfo( Extent3D extent )
    {
      ImageCreateInfo createInfo( { },                                                                  // flags
                                  ImageType::e2D,                                                       // imageType
                                  Format::eR8G8B8A8Srgb,                                                // format
                                  extent,                                                               // extent
                                  1u,                                                                   // mipLevels
                                  1u,                                                                   // arrayLayers
                                  SampleCountFlagBits::e1,                                              // samples
                                  ImageTiling::eOptimal,                                                // tiling
                                  ImageUsageFlagBits::eTransferDst | ImageUsageFlagBits::eSampled,      // usage
                                  SharingMode::eExclusive,                                              // sharingMode
                                  0u,                                                                   // queueFamilyIndexCount
                                  nullptr,                                                              // pQueueFamilyIndices
                                  ImageLayout::eUndefined );                                            // initialLayout

      return createInfo;
    }

    SamplerCreateInfo getSamplerCreateInfo( )
    {
      SamplerCreateInfo createInfo( { },                            // flags
                                    Filter::eLinear,                // magFilter
                                    Filter::eLinear,                // minFilter
                                    SamplerMipmapMode::eLinear,     // mipmapMode
                                    SamplerAddressMode::eRepeat,    // addressModeU
                                    SamplerAddressMode::eRepeat,    // addressModeV
                                    SamplerAddressMode::eRepeat,    // addressModeW
                                    { },                            // mipLodBias,
                                    VK_TRUE,                        // anisotropyEnable
                                    16.0f,                          // maxAnisotropy
                                    VK_FALSE,                       // compareEnable
                                    CompareOp::eAlways,             // compareOp
                                    { },                            // minLod
                                    { },                            // maxLod
                                    BorderColor::eIntOpaqueBlack,   // borderColor
                                    VK_FALSE );                     // unnormalizedCoordinates
      return createInfo;
    }

    SubmitInfo getSubmitInfo( const Semaphore& waitSemaphore, const Semaphore& signalSemaphore, const std::vector<CommandBuffer>& commandBuffers, const PipelineStageFlags& pWaitDstStageMask )
    {
      return SubmitInfo( 1,                                               // waitSemaphoreCount
                         &waitSemaphore,                                  // pWaitSemaphores
                         &pWaitDstStageMask,                              // pWaitDstStageMask
                         static_cast<uint32_t>( commandBuffers.size( ) ), // commandBufferCount
                         commandBuffers.data( ),                          // pCommandBuffers
                         1,                                               // signalSemaphoreCount
                         &signalSemaphore );                              // pSignalSemaphores
    }

    PresentInfoKHR getPresentInfoKHR( const Semaphore& waitSemaphore, uint32_t& imageIndex )
    {
      return PresentInfoKHR( 1,                // waitSemaphoreCount
                             &waitSemaphore,   // pWaitSemaphores
                             1,                // swapchainCount
                             &rx::g_swapchain, // pSwapchains
                             &imageIndex,      // pImageIndices
                             nullptr );        // pResults
    }

    uint32_t findMemoryType( PhysicalDevice physicalDevice, uint32_t typeFilter, MemoryPropertyFlags properties )
    {
      static PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties( );

      for ( uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i )
      {
        if ( typeFilter & ( 1 << i ) && ( memoryProperties.memoryTypes[i].propertyFlags & properties ) == properties )
          return i;
      }

      RX_ERROR( "Failed to find suitable memory type" );
      return 0u;
    }

    std::vector<char> parseShader( const std::string& path )
    {
      std::string delimiter = "/";
      std::string pathToFile = "";
      std::string fileName = "";
      std::string fileNameOut = "";

      size_t pos = path.find_last_of( delimiter );
      if ( pos != std::string::npos )
      {
        pathToFile = path.substr( 0, pos + 1 ).c_str( );
        fileName = path.substr( pos + 1 ).c_str( );
      }
      else
        RX_ERROR( "Can not process shader paths." );

      // This is the name of the resulting shader.
      // For example, myShader.frag will turn into myShader_frag.spv.
      fileNameOut = fileName;
      delimiter = ".";

      pos = fileName.find_last_of( delimiter );
      if ( pos != std::string::npos )
      {
        std::replace( fileNameOut.begin( ), fileNameOut.end( ), '.', '_' );
        fileNameOut += ".spv";
      }
      else
        RX_ERROR( "Can not process shader file name." );

      // Calls glslc to compile the glsl file into spir-v.
      std::stringstream command;
      command << "cd " << pathToFile << " && " << RX_GLSLC_PATH << " " << fileName << " -o " << fileNameOut;
      std::system( command.str( ).c_str( ) );

      // Read the file and retrieve the source.
      std::ifstream file( pathToFile + fileNameOut, std::ios::ate | std::ios::binary );

      if ( !file.is_open( ) )
        RX_ERROR( "Failed to open shader source file." );

      size_t fileSize = static_cast< size_t >( file.tellg( ) );
      std::vector<char> buffer( fileSize );

      file.seekg( 0 );
      file.read( buffer.data( ), fileSize );

      file.close( );

      return buffer;
    }

    std::vector<ImageView> unpack( const std::vector<UniqueImageView>& uniqueImageViews )
    {
      std::vector<ImageView> result( uniqueImageViews.size( ) );

      for ( size_t i = 0; i < result.size( ); ++i )
        result[i] = uniqueImageViews[i].get( );

      return result;
    }

    std::vector<Framebuffer> unpack( const std::vector<UniqueFramebuffer>& uniqueFramebuffers )
    {
      std::vector<Framebuffer> result( uniqueFramebuffers.size( ) );

      for ( size_t i = 0; i < result.size( ); ++i )
        result[i] = uniqueFramebuffers[i].get( );

      return result;
    }

    std::vector<std::shared_ptr<rx::Model>> unpack( const std::unordered_map<std::string, std::shared_ptr<rx::Model>>& models )
    {
      std::vector<std::shared_ptr<rx::Model>> result;
      result.reserve( models.size( ) );

      for ( const auto& model : models )
        result.push_back( model.second );

      return result;
    }

    std::vector<DescriptorPoolSize> getPoolSizes( const std::vector<DescriptorSetLayoutBinding>& layoutBindings )
    {
      std::vector<DescriptorPoolSize> result;
      result.reserve( layoutBindings.size( ) );

      for ( const auto& layoutBinding : layoutBindings )
      {
        DescriptorPoolSize poolSize( layoutBinding.descriptorType,    // type
                                     layoutBinding.descriptorCount ); // count

        result.push_back( poolSize );
      }

      return result;
    }


    void transitionImageLayout( Image image, ImageLayout oldLayout, ImageLayout newLayout )
    {
      auto barrierInfo = getImageMemoryBarrierInfo( image, oldLayout, newLayout );

      rx::CommandBuffer commandBuffer;
      commandBuffer.init( rx::g_graphicsCmdPool );
      commandBuffer.begin( );

      commandBuffer.get( 0 ).pipelineBarrier( std::get<1>( barrierInfo ),        // srcStageMask
                                              std::get<2>( barrierInfo ),        // dstStageMask
                                              DependencyFlagBits::eByRegion,
                                              0,
                                              nullptr,
                                              0,
                                              nullptr,
                                              1,
                                              &std::get<0>( barrierInfo ) );     // barrier

      commandBuffer.end( );
      commandBuffer.submitToQueue( rx::g_graphicsQueue );
    }

    void transitionImageLayout( Image image, ImageLayout oldLayout, ImageLayout newLayout, CommandBuffer commandBuffer )
    {
      auto barrierInfo = getImageMemoryBarrierInfo( image, oldLayout, newLayout );

      commandBuffer.pipelineBarrier( std::get<1>( barrierInfo ),        // srcStageMask
                                     std::get<2>( barrierInfo ),        // dstStageMask
                                     DependencyFlagBits::eByRegion,
                                     0,
                                     nullptr,
                                     0,
                                     nullptr,
                                     1,
                                     &std::get<0>( barrierInfo ) );     // barrier
    }

    std::tuple<ImageMemoryBarrier, PipelineStageFlags, PipelineStageFlags> getImageMemoryBarrierInfo( Image image, ImageLayout oldLayout, ImageLayout newLayout )
    {
      // TODO: not style conform.
      ImageMemoryBarrier barrier;
      barrier.oldLayout = oldLayout;
      barrier.newLayout = newLayout;
      barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barrier.image = image;
      barrier.subresourceRange.aspectMask = ImageAspectFlagBits::eColor;
      barrier.subresourceRange.baseMipLevel = 0;
      barrier.subresourceRange.levelCount = 1;
      barrier.subresourceRange.baseArrayLayer = 0;
      barrier.subresourceRange.layerCount = 1;

      PipelineStageFlags srcStageMask = PipelineStageFlagBits::eAllCommands;
      PipelineStageFlags dstStageMask = PipelineStageFlagBits::eAllCommands;

      if ( oldLayout == ImageLayout::eUndefined && newLayout == ImageLayout::eTransferDstOptimal )
      {
        barrier.srcAccessMask = { };
        barrier.dstAccessMask = AccessFlagBits::eTransferWrite;

        srcStageMask = PipelineStageFlagBits::eTopOfPipe;
        dstStageMask = PipelineStageFlagBits::eTransfer;
      }
      else if ( oldLayout == ImageLayout::eTransferDstOptimal && newLayout == ImageLayout::eShaderReadOnlyOptimal )
      {
        barrier.srcAccessMask = AccessFlagBits::eTransferWrite;
        barrier.dstAccessMask = AccessFlagBits::eShaderRead;

        srcStageMask = PipelineStageFlagBits::eTransfer;
        dstStageMask = PipelineStageFlagBits::eFragmentShader;
      }
      else if ( oldLayout == ImageLayout::eUndefined && newLayout == ImageLayout::eGeneral ) 
      {
        // nothing to do.
      }
      else if ( oldLayout == ImageLayout::eGeneral && newLayout == ImageLayout::eTransferSrcOptimal )
      {
        barrier.dstAccessMask = AccessFlagBits::eTransferRead;
      }
      else if ( oldLayout == ImageLayout::eTransferSrcOptimal && newLayout == ImageLayout::eGeneral )
      {
        barrier.srcAccessMask = AccessFlagBits::eTransferRead;
      }
      else if ( oldLayout == ImageLayout::eTransferDstOptimal && newLayout == ImageLayout::ePresentSrcKHR )
      {
        barrier.srcAccessMask = AccessFlagBits::eTransferWrite;
      }
      else if ( oldLayout == ImageLayout::eUndefined && newLayout == ImageLayout::ePresentSrcKHR )
      {
        barrier.srcAccessMask = { };
      }
      else if ( oldLayout == ImageLayout::ePresentSrcKHR && newLayout == ImageLayout::eColorAttachmentOptimal )
      {
        barrier.dstAccessMask = AccessFlagBits::eColorAttachmentWrite;
      }
      else if ( oldLayout == ImageLayout::eColorAttachmentOptimal && newLayout == ImageLayout::ePresentSrcKHR )
      {
        barrier.srcAccessMask = AccessFlagBits::eColorAttachmentWrite;
      }
      else if ( oldLayout == ImageLayout::eUndefined && newLayout == ImageLayout::eColorAttachmentOptimal )
      {
        barrier.srcAccessMask = { };
        barrier.dstAccessMask = AccessFlagBits::eColorAttachmentWrite;
      }
      else if ( oldLayout == ImageLayout::eTransferDstOptimal && newLayout == ImageLayout::eColorAttachmentOptimal )
      {
        barrier.srcAccessMask = AccessFlagBits::eTransferWrite;
        barrier.dstAccessMask = AccessFlagBits::eColorAttachmentWrite;
      }
      else
        RX_ERROR( "Image layout transition not supported." );

      return { barrier, srcStageMask, dstStageMask };
    }

    AttachmentDescription getAttachmentDescription( Format surfaceFormat, bool guiEnabled )
    {
      return vk::AttachmentDescription( { },                                                                               // flags
                                        surfaceFormat,                                                                     // format
                                        SampleCountFlagBits::e1,                                                           // samples
                                        AttachmentLoadOp::eClear,                                                          // loadOp
                                        AttachmentStoreOp::eStore,                                                         // storeOp
                                        AttachmentLoadOp::eDontCare,                                                       // stencilLoadOp
                                        AttachmentStoreOp::eDontCare,                                                      // stencilStoreOp
                                        ImageLayout::eUndefined,                                                           // initialLayout
                                        guiEnabled ? ImageLayout::eColorAttachmentOptimal : ImageLayout::ePresentSrcKHR ); // finalLayout  
    }

    AttachmentDescription getDepthAttachmentDescription( Format depthFormat )
    {
      return AttachmentDescription( { },                                           // flags
                                    depthFormat,                                   // format
                                    SampleCountFlagBits::e1,                       // samples
                                    AttachmentLoadOp::eClear,                      // loadOp
                                    AttachmentStoreOp::eStore,                     // storeOp
                                    AttachmentLoadOp::eClear,                      // stencilLoadOp
                                    AttachmentStoreOp::eDontCare,                  // stencilStoreOp
                                    ImageLayout::eUndefined,                       // initialLayout
                                    ImageLayout::eDepthStencilAttachmentOptimal ); // finalLayout    
    }
  }
}