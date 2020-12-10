#version 460
#extension GL_EXT_ray_tracing : require

#include "Ray.glsl"

layout( location = 0 ) rayPayloadInEXT RayPayLoad ray;

layout( push_constant ) uniform Constants
{
  vec4 clearColor;
  int frameCount;
  uint sampleRatePerPixel;
  uint maxRecursionDepth;
  bool useEnvironmentMap;

  uint padding0;
  uint padding1;
  uint padding2;
  uint padding3;
};

layout( binding = 2, set = 1 ) uniform samplerCube environmentMap;

void main( )
{
  //ray.hitValue = clearColor.xyz * 0.8; // * clearColor.w;
  //return;

  //ray.hitValue = texture( skybox, ray.rayDirection ).xyz

  // If the ray hits nothing right away
  if ( ray.depth == 0 )
  {
    if ( useEnvironmentMap )
    {
      ray.hitValue = texture( environmentMap, ray.rayDirection ).xyz * 0.8F;
    }
    else
    {
      ray.hitValue = clearColor.xyz * clearColor.w;
    }
  }
  else
  {
    ray.hitValue = vec3( 0.01 ); // Tiny contribution from environment
  }

  ray.depth = maxRecursionDepth + 1; // Ending trace
}
