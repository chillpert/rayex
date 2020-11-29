#version 460
#extension GL_EXT_ray_tracing : require

#include "Ray.glsl"

layout( location = 0 ) rayPayloadInEXT hitPayload prd;

layout( push_constant ) uniform Constants
{
  vec4 clearColor;
  uint frameCount;
  uint sampleRatePerPixel;
  uint maxRecursionDepth;
  uint directionalLightCount;
  uint pointLightCount;
  bool useEnvironmentMap;

  uint padding0;
  uint padding1;
};

layout( binding = 3, set = 2 ) uniform samplerCube environmentMap;

void main( )
{
  //prd.hitValue = clearColor.xyz * 0.8; // * clearColor.w;
  //return;

  //prd.hitValue = texture( skybox, -prd.rayDirection ).xyz

  // If the ray hits nothing right away
  if ( prd.depth == 0 )
  {
    if ( useEnvironmentMap )
    {
      prd.hitValue = texture( environmentMap, -prd.rayDirection ).xyz;
    }
    else
    {
      prd.hitValue = clearColor.xyz * clearColor.w;
    }
  }
  else
  {
    // This will make it so that the skybox can be seen through objects. At least it used to. Now it looks like it's just kind of
    // giving a small contribution to the final color. Which kind of makes sense.
    // prd.hitValue = texture( environmentMap, -prd.rayDirection ).xyz * 0.1

    prd.hitValue = vec3( 0.01 ); // Tiny contribution from environment
  }

  prd.depth = maxRecursionDepth + 1; // Ending trace
}
