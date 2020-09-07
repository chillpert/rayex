#version 460
#extension GL_EXT_ray_tracing : require

#include "raycommon.glsl"

layout(location = 0) rayPayloadInEXT vec3 hitValue;

void main()
{
  hitValue = vec3(0.45, 0.45, 0.45);
}