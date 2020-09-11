#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable

#include "raycommon.glsl"
#include "lights.glsl"

layout(location = 0) rayPayloadInEXT vec3 hitValue;
hitAttributeEXT vec3 attribs;

layout(binding = 0, set = 1) uniform sampler2D texSampler;

layout(binding = 0, set = 2) uniform LightSources
{
  DirectionalLight[10] directionalLights;
  PointLight[10] pointLights;
} lightSources;

void main()
{
  if ( lightSources.directionalLights[0].exists == 1.0f )
  {
    hitValue = vec3(1.0, 0.0, 0.0);
  }
  else if ( lightSources.directionalLights[0].exists != 1.0f )
  {
    hitValue = vec3(0.0, 1.0, 0.0);
  }
  else 
  {
    hitValue = vec3(0.3, 0.3, 0.3);
  }
}
