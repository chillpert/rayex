#include "api/vulkan/GraphicsPipeline.hpp"
#include "GraphicsPipeline.hpp"
#include "api/Shader.hpp"

namespace RX
{
  void GraphicsPipeline::createGraphicsPipeline()
  {
    // trying both constructors
    Shader fragShader(RX_SHADER_PATH, "test.frag");
    Shader vertShader(RX_PROJECT_PATH "resources/shaders/test.vert");
  }
}