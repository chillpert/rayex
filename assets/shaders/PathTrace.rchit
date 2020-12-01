#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_nonuniform_qualifier : enable

#include "Instance.glsl"
#include "Lights.glsl"
#include "Random.glsl"
#include "Ray.glsl"

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

layout( location = 0 ) rayPayloadInEXT RayPayLoad ray;
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

layout( binding = 0, set = 2 ) buffer Vertices
{
  vec4 v[];
}
vertices[];

layout( binding = 1, set = 2 ) buffer Indices
{
  uint i[];
}
indices[];

layout( binding = 2, set = 2 ) buffer Meshes
{
  Mesh m[];
}
meshes[];

layout( binding = 4, set = 2 ) uniform sampler2D textures[];

layout( push_constant ) uniform Constants
{
  vec4 clearColor;
  uint frameCount;
  uint sampleRatePerPixel;
  uint maxRecursionDepth;
  uint directionalLightCount;
  uint pointLightCount;
  bool uintuseEnvironmentMap;

  uint padding0;
  uint padding1;
};

Vertex unpackVertex( uint index, uint modelIndex )
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

  Vertex v0 = unpackVertex( ind.x, modelIndex );
  Vertex v1 = unpackVertex( ind.y, modelIndex );
  Vertex v2 = unpackVertex( ind.z, modelIndex );

  const vec3 barycentrics = vec3( 1.0 - attribs.x - attribs.y, attribs.x, attribs.y );
  // Computing the normal at hit position
  vec3 normal = v0.normal * barycentrics.x + v1.normal * barycentrics.y + v2.normal * barycentrics.z;
  // Transforming the normal to world space
  normal = normalize( vec3( geometryInstances.i[gl_InstanceID].transformIT * vec4( normal, 0.0 ) ) );

  // Computing the coordinates of the hit position
  vec3 worldPos = v0.pos * barycentrics.x + v1.pos * barycentrics.y + v2.pos * barycentrics.z;
  // Transforming the position to world space
  worldPos = vec3( geometryInstances.i[gl_InstanceID].transform * vec4( worldPos, 1.0 ) );

  vec2 texCoord = v0.texCoord * barycentrics.x + v1.texCoord * barycentrics.y + v2.texCoord * barycentrics.z;

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

  // Diffuse lighting
  vec3 diffuse  = vec3( 1.0 );
  vec3 emission = vec3( 0.0 ); // emittance / emissiveFactor

  if ( found )
  {
    emission = mat.emission.xyz;

    // No texture assigned.
    if ( mat.diffuse.w == -1.0F )
    {
      diffuse = mat.diffuse.xyz;
    }
    // Texture assigned.
    else
    {
      diffuse = mat.diffuse.xyz + texture( textures[nonuniformEXT( int( mat.diffuse.w ) )], texCoord ).xyz;
    }
  }

  // Pick a random direction from here and keep going.
  vec3 tangent, bitangent;
  createCoordinateSystem( normal, tangent, bitangent );
  vec3 rayOrigin    = worldPos;
  vec3 rayDirection = samplingHemisphere( ray.seed, tangent, bitangent, normal );

  // Probability of the new ray (cosine distributed)
  const float p = 1 / M_PI;

  // Compute the BRDF for this ray (assuming Lambertian reflection)
  float cos_theta = dot( rayDirection, normal );
  vec3 BRDF       = diffuse.xyz / M_PI;

  ray.rayOrigin    = rayOrigin;
  ray.rayDirection = rayDirection;
  ray.weight       = BRDF * cos_theta / p;
  ray.hitValue     = emission;
}
