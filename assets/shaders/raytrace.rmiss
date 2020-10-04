#version 460
#extension GL_EXT_ray_tracing : require

#include "raycommon.glsl"

layout(location = 0) rayPayloadInEXT hitPayload prd;

layout(push_constant) uniform Constants
{
  vec4 clearColor;
};

void main()
{
  prd.hitValue = clearColor.xyz * clearColor.w;
}