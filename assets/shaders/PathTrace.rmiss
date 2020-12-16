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
    if ( ray.reflective )
    {
      ray.reflective = false;

      if ( useEnvironmentMap )
      {
        ray.hitValue = texture( environmentMap, ray.rayDirection ).xyz;
      }
      else
      {
        ray.hitValue = clearColor.xyz * clearColor.w;
      }
    }
    else
    {
      // Tiny contribution from environment
      ray.hitValue = vec3( 0.01 );
    }
  }

  // End the path
  ray.depth = maxRecursionDepth + 1;
}
