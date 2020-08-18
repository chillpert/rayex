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
                                    BorderColor::eIntOpaqueBlack,  // borderColor
                                    VK_FALSE );                     // unnormalizedCoordinates
      return createInfo;
    }

    uint32_t findType( PhysicalDevice physicalDevice, uint32_t typeFilter, MemoryPropertyFlags properties )
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

    std::vector<vk::ImageView> unpack( const std::vector<vk::UniqueImageView>& uinqueImageViews )
    {
      std::vector<vk::ImageView> result( uinqueImageViews.size( ) );

      for ( size_t i = 0; i < result.size( ); ++i )
        result[i] = uinqueImageViews[i].get( );

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
                                              vk::DependencyFlagBits::eByRegion,
                                              0,
                                              nullptr,
                                              0,
                                              nullptr,
                                              1,
                                              &std::get<0>( barrierInfo ) );     // barrier

      commandBuffer.end( );
      commandBuffer.submitToQueue( rx::g_graphicsQueue );
    }

    void transitionImageLayout( Image image, ImageLayout oldLayout, ImageLayout newLayout, vk::CommandBuffer commandBuffer )
    {
      auto barrierInfo = getImageMemoryBarrierInfo( image, oldLayout, newLayout );

      commandBuffer.pipelineBarrier( std::get<1>( barrierInfo ),        // srcStageMask
                                       std::get<2>( barrierInfo ),        // dstStageMask
                                       vk::DependencyFlagBits::eByRegion,
                                       0,
                                       nullptr,
                                       0,
                                       nullptr,
                                       1,
                                       &std::get<0>( barrierInfo ) );     // barrier
    }

    std::tuple<vk::ImageMemoryBarrier, vk::PipelineStageFlags, vk::PipelineStageFlags> getImageMemoryBarrierInfo( vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout )
    {
      // TODO: not style conform.
      vk::ImageMemoryBarrier barrier;
      barrier.oldLayout = oldLayout;
      barrier.newLayout = newLayout;
      barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barrier.image = image;
      barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
      barrier.subresourceRange.baseMipLevel = 0;
      barrier.subresourceRange.levelCount = 1;
      barrier.subresourceRange.baseArrayLayer = 0;
      barrier.subresourceRange.layerCount = 1;

      vk::PipelineStageFlags srcStageMask = vk::PipelineStageFlagBits::eAllCommands;
      vk::PipelineStageFlags dstStageMask = vk::PipelineStageFlagBits::eAllCommands;

      if ( oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal )
      {
        barrier.srcAccessMask = { };
        barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

        srcStageMask = vk::PipelineStageFlagBits::eTopOfPipe;
        dstStageMask = vk::PipelineStageFlagBits::eTransfer;
      }
      else if ( oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal )
      {
        barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
        barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

        srcStageMask = vk::PipelineStageFlagBits::eTransfer;
        dstStageMask = vk::PipelineStageFlagBits::eFragmentShader;
      }
      else if ( oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eGeneral ) 
      {
        // nothing to do.
      }
      else if ( oldLayout == vk::ImageLayout::eGeneral && newLayout == vk::ImageLayout::eTransferSrcOptimal )
      {
        barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;
      }
      else if ( oldLayout == vk::ImageLayout::eTransferSrcOptimal && newLayout == vk::ImageLayout::eGeneral )
      {
        barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
      }
      else if ( oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::ePresentSrcKHR )
      {
        barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
      }
      else if ( oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::ePresentSrcKHR )
      {
        barrier.srcAccessMask = { };
      }
      else
        RX_ERROR( "Image layout transition not supported." );

      return { barrier, srcStageMask, dstStageMask };
    }
  }
}