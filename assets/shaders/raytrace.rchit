#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_nonuniform_qualifier : enable

#include "instance.glsl"
#include "lights.glsl"
#include "raycommon.glsl"

#define TOTAL_DIRECTIONAL_LIGHTS 1
#define TOTAL_POINT_LIGHTS       1

#define TOTAL_MODELS 1

struct Vertex
{
  vec3 pos;
  vec3 normal;
  vec3 color;
  vec2 texCoord;

  float padding0;
};

hitAttributeEXT vec3 attribs;

layout( location = 0 ) rayPayloadInEXT hitPayload prd;
layout( location = 1 ) rayPayloadEXT bool isShadowed;

layout( binding = 0, set = 0 ) uniform accelerationStructureEXT topLevelAS;

layout( binding = 1, set = 1 ) buffer DirectionalLights
{
  DirectionalLight directionalLights[];
};

layout( binding = 2, set = 1 ) buffer PointLights
{
  PointLight pointLights[];
};

layout( binding = 3, set = 1 ) buffer GeometryInstances
{
  GeometryInstance i[];
}
geometryInstances;

layout( binding = 4, set = 1 ) buffer Meshes
{
  Mesh m[];
}
meshes[];

layout( binding = 0, set = 2 ) buffer Vertices
{
  vec4 v[];
}
vertices[];

layout( binding = 0, set = 3 ) buffer Indices
{
  uint i[];
}
indices[];

layout( push_constant ) uniform Constants
{
  vec4 clearColor;
  uint frameCount;
  uint jitterCamSampleRatePerRayGen;
  uint ssaa;
  bool jitterCamEnabled;
  bool ssaaEnabled;

  uint directionalLightCount;
  uint pointLightCount;
  float padding0;
};

Vertex decompressVertex( uint index, uint modelIndex )
{
  vec4 d0 = vertices[nonuniformEXT( modelIndex )].v[3 * index + 0];
  vec4 d1 = vertices[nonuniformEXT( modelIndex )].v[3 * index + 1];
  vec4 d2 = vertices[nonuniformEXT( modelIndex )].v[3 * index + 2];

  Vertex v;
  v.pos      = d0.xyz;
  v.normal   = vec3( d0.w, d1.x, d1.y );
  v.color    = vec3( d1.z, d1.w, d2.x );
  v.texCoord = vec2( d2.y, d2.z );
  return v;
}

void main( )
{
  uint modelIndex = geometryInstances.i[gl_InstanceID].modelIndex;

  ivec3 ind = ivec3( indices[nonuniformEXT( modelIndex )].i[3 * gl_PrimitiveID + 0],   //
                     indices[nonuniformEXT( modelIndex )].i[3 * gl_PrimitiveID + 1],   //
                     indices[nonuniformEXT( modelIndex )].i[3 * gl_PrimitiveID + 2] ); //

  Vertex v0 = decompressVertex( ind.x, modelIndex );
  Vertex v1 = decompressVertex( ind.y, modelIndex );
  Vertex v2 = decompressVertex( ind.z, modelIndex );

  const vec3 barycentrics = vec3( 1.0 - attribs.x - attribs.y, attribs.x, attribs.y );
  vec3 normal             = v0.normal * barycentrics.x + v1.normal * barycentrics.y + v2.normal * barycentrics.z;
  normal                  = normalize( vec3( geometryInstances.i[gl_InstanceID].transformIT * vec4( normal, 0.0 ) ) );

  // Calculate world space position (the unprecise way)
  //vec3 worldPos = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT;

  // or better like this:
  // Computing the coordinates of the hit position
  vec3 worldPos = v0.pos * barycentrics.x + v1.pos * barycentrics.y + v2.pos * barycentrics.z;
  // Transforming the position to world space
  worldPos = vec3( geometryInstances.i[gl_InstanceID].transform * vec4( worldPos, 1.0 ) );

  vec3 L              = vec3( 0.0 );
  float lightDistance = 100000.0;
  float attenuation   = 1;

  if ( directionalLightCount > 0 )
  {
    L = normalize( directionalLights[0].direction.xyz );
  }
  else
  {
    prd.hitValue = vec3( 0.0 );
    return;
  }

  // Tracing shadow ray only if the light is visible from the surface
  if ( dot( normal, L ) > 0 )
  {
    float tMin  = 0.001;
    float tMax  = lightDistance;
    vec3 origin = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT;
    vec3 rayDir = L;
    uint flags  = gl_RayFlagsTerminateOnFirstHitEXT | gl_RayFlagsOpaqueEXT | gl_RayFlagsSkipClosestHitShaderEXT;
    isShadowed  = true;
    traceRayEXT( topLevelAS, // acceleration structure
                 flags,      // rayFlags
                 0xFF,       // cullMask
                 0,          // sbtRecordOffset
                 0,          // sbtRecordStride
                 1,          // missIndex
                 origin,     // ray origin
                 tMin,       // ray min range
                 rayDir,     // ray direction
                 tMax,       // ray max range
                 1           // payload (location = 1)
    );

    if ( isShadowed )
    {
      attenuation = 0.3;
    }
  }

  // The following lines access the meshes SSBO to figure out to which submesh the current face belongs and retrieve its material.
  Material mat;
  bool found       = false;
  int subMeshCount = meshes[nonuniformEXT( modelIndex )].m.length( );

  for ( int i = 0; i < subMeshCount; ++i )
  {
    uint offset     = meshes[nonuniformEXT( modelIndex )].m[i].indexOffset;
    uint prevOffset = 0;

    if ( i > 0 )
    {
      prevOffset = meshes[nonuniformEXT( modelIndex )].m[i - 1].indexOffset;
    }

    if ( gl_PrimitiveID < offset && gl_PrimitiveID >= prevOffset )
    {
      mat   = meshes[nonuniformEXT( modelIndex )].m[i].material;
      found = true;
      break;
    }
  }

  vec3 diffuse  = vec3( 1.0 );
  vec2 texCoord = v0.texCoord * barycentrics.x + v1.texCoord * barycentrics.y + v2.texCoord * barycentrics.z;

  if ( found )
  {
    diffuse = mat.diffuse.xyz;
  }

  float dotNL  = max( dot( normal, L ), 0.2 );
  prd.hitValue = vec3( dotNL ) * diffuse * attenuation;
}
