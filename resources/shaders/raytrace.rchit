#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable

#include "raycommon.glsl"

layout(location = 0) rayPayloadInEXT vec3 hitValue;
hitAttributeEXT vec3 attribs;

layout(binding = 0, set = 1) uniform sampler2D texSampler;

void main()
{
  hitValue = vec3(0.3, 0.3, 0.3);
}
