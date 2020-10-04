#version 460
#extension GL_ARB_separate_shader_objects : enable

#include "lights.glsl"

#define TOTAL_DIRECTIONAL_LIGHTS 1
#define TOTAL_POINT_LIGHTS 1

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 cameraPos;

layout(location = 0) out vec4 outColor;

layout(binding = 1, set = 0) uniform LightSources
{
  DirectionalLight directionalLights[TOTAL_DIRECTIONAL_LIGHTS];
  PointLight pointLights[TOTAL_POINT_LIGHTS];
} lightSources;

void main()
{
  vec3 L = normalize( lightSources.directionalLights[0].direction.xyz - vec3( 0 ) );
  float dotNL = max(dot(fragNormal, L), 0.2 );

  outColor = vec4( vec3( dotNL ), 1.0);
}