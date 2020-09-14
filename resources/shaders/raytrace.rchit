#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_nonuniform_qualifier : enable

#include "raycommon.glsl"
#include "lights.glsl"

layout(location = 0) rayPayloadInEXT vec3 hitValue;
hitAttributeEXT vec3 attribs;

layout(binding = 0, set = 1) uniform sampler2D texSampler;

struct Vertex
{
  vec3 pos;
  vec3 color;
  vec3 normal;
  vec2 texCoord;
};

struct SceneDescription
{
  int  objId;
  int  txtOffset;
  mat4 transfo;
  mat4 transfoIT;
};

layout(binding = 1, set = 1) buffer Vertices
{
  Vertex v[];
} vertices[];

layout(binding = 2, set = 1) buffer Indices
{
  uint i[];
} indices[];

layout(binding = 3, set = 1) buffer SceneDescriptions
{
  SceneDescription i[];
} sceneDescription;

layout(binding = 0, set = 2) uniform LightSources
{
  DirectionalLight directionalLights[10];
  PointLight pointLights[10];
} lightSources;

layout(push_constant) uniform Constants
{
  vec4 clearColor;
};


void main()
{
  if ( lightSources.directionalLights[0].direction.w == 1.0f )
  {
    hitValue = vec3(1.0, 0.0, 0.0);
  }
  else if ( lightSources.directionalLights[0].direction.w != 1.0f )
  {
    hitValue = vec3(0.0, 1.0, 0.0);
  }
  else 
  {
    hitValue = vec3(0.3, 0.3, 0.3);
  }
}
