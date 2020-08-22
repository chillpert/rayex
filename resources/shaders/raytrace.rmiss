#version 460
#extension GL_NV_ray_tracing : require

struct hitPayload
{
  vec3 hitValue;
};

layout(location = 0) rayPayloadInNV hitPayload prd;

layout(push_constant) uniform Constants
{
  vec4 clearColor;
};

void main()
{
  prd.hitValue = clearColor.xyz * 0.8;
}
