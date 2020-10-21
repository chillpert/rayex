#include "api/utility/Util.hpp"

#include "api/Components.hpp"

namespace RAYEX_NAMESPACE::Util
{
  auto parseShader( std::string_view path ) -> std::vector<char>
  {
    std::string delimiter = "/";
    std::string pathToFile;
    std::string fileName;
    std::string fileNameOut;

    size_t pos = path.find_last_of( delimiter );
    if ( pos != std::string::npos )
    {
      pathToFile = std::string( path.substr( 0, pos + 1 ) );
      fileName   = std::string( path.substr( pos + 1 ) );
    }
    else
    {
      RX_ERROR( "Can not process shader paths." );
    }

    // This is the name of the resulting shader.
    // For example, myShader.frag will turn into myShader_frag.spv.
    fileNameOut = fileName;
    delimiter   = ".";

    pos = fileName.find_last_of( delimiter );
    if ( pos != std::string::npos )
    {
      std::replace( fileNameOut.begin( ), fileNameOut.end( ), '.', '_' );
      fileNameOut += ".spv";
    }
    else
    {
      RX_ERROR( "Can not process shader file name." );
    }

    // Calls glslc to compile the glsl file into spir-v.
    std::stringstream command;
    //command << "cd " << pathToFile << " && " << RX_GLSLC_PATH << " " << fileName << " -o " << fileNameOut << " --target-env=vulkan1.2";
    command << RX_GLSLC_PATH << " " << components::assetsPath << "shaders/" << fileName << " -o " << components::assetsPath << "shaders/" << fileNameOut << " --target-env=vulkan1.2";

    std::system( command.str( ).c_str( ) );

    // Read the file and retrieve the source.
    std::string pathToShaderSourceFile = components::assetsPath + pathToFile + fileNameOut;
    std::ifstream file( pathToShaderSourceFile, std::ios::ate | std::ios::binary );

    if ( !file.is_open( ) )
    {
      RX_ERROR( "Failed to open shader source file ", pathToShaderSourceFile );
    }

    size_t fileSize = static_cast<size_t>( file.tellg( ) );
    std::vector<char> buffer( fileSize );

    file.seekg( 0 );
    file.read( buffer.data( ), fileSize );

    file.close( );

    return buffer;
  }

  void processShaderMacros( std::string_view path, uint32_t dirLightNodes, uint32_t pointLightNodes, uint32_t totalModels )
  {
    std::string pathToFile = components::assetsPath + std::string( path );
    std::ifstream file( pathToFile );

    if ( !file.is_open( ) )
    {
      RX_ERROR( "Failed to open shader source file ", pathToFile );
    }

    std::string dirLightSearchTag    = "#define TOTAL_DIRECTIONAL_LIGHTS";
    std::string pointLightSearchTag  = "#define TOTAL_POINT_LIGHTS";
    std::string totalModelsSearchTag = "#define TOTAL_MODELS";

    std::string line;
    std::string processedContent;
    while ( std::getline( file, line ) )
    {
      if ( line.find( dirLightSearchTag ) != std::string::npos )
      {
        auto pos = line.find_last_of( " " );
        if ( pos != std::string::npos )
        {
          line.replace( pos + 1, line.length( ), std::to_string( dirLightNodes ) );
        }
      }

      if ( line.find( pointLightSearchTag ) != std::string::npos )
      {
        auto pos = line.find_last_of( " " );
        if ( pos != std::string::npos )
        {
          line.replace( pos + 1, line.length( ), std::to_string( pointLightNodes ) );
        }
      }

      if ( line.find( totalModelsSearchTag ) != std::string::npos )
      {
        auto pos = line.find_last_of( " " );
        if ( pos != std::string::npos )
        {
          line.replace( pos + 1, line.length( ), std::to_string( totalModels ) );
        }
      }

      processedContent += line + "\n";
    }

    file.close( );

    std::ofstream out( pathToFile );
    out << processedContent;
    out.close( );
  }

  auto vec4toArray( const glm::vec4& vec ) -> std::array<float, 4>
  {
    return { vec.x, vec.y, vec.z, vec.w };
  }
} // namespace RAYEX_NAMESPACE::Util
