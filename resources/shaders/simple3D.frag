#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "lights.glsl"

#define TOTAL_DIRECTIONAL_LIGHTS 1
#define TOTAL_POINT_LIGHTS 1

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 cameraPos;

layout(location = 0) out vec4 outColor;

layout(binding = 1, set = 0) uniform sampler2D texSampler;

layout(binding = 0, set = 1) uniform LightSources
{
  DirectionalLight directionalLights[TOTAL_DIRECTIONAL_LIGHTS];
  PointLight pointLights[TOTAL_POINT_LIGHTS];
} lightSources;

vec3 calcDirLight(vec3 normal, vec3 fragPos, vec3 viewDir)
{
  // ambient
  vec3 ambient = 0.5 * vec3(texture(texSampler, fragTexCoord));

  // diffuse
  vec3 lightDir = normalize(vec3(0, 10.0, 10.0));
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diff * vec3(texture(texSampler, fragTexCoord));

  return (ambient + diffuse);
}

void main()
{
  vec3 L = normalize( lightSources.directionalLights[0].direction.xyz - vec3( 0 ) );
  float dotNL = max(dot(fragNormal, L), 0.2 );

  outColor = vec4( vec3( dotNL ), 1.0);

  //outColor = vec4(vec3(texture(texSampler, fragTexCoord)), 1.0);

  //vec3 norm = fragNormal;
  //vec3 fragPos = fragPos;
  //vec3 viewDir = normalize(cameraPos - fragPos);
  //
  //outColor = vec4(calcDirLight(norm, fragPos, viewDir), 1.0);
}