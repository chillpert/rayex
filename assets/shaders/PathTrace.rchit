#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_nonuniform_qualifier : enable

#include "Geometry.glsl"
#include "Instance.glsl"
#include "Random.glsl"
#include "Ray.glsl"

hitAttributeEXT vec3 attribs;

layout( location = 0 ) rayPayloadInEXT RayPayLoad ray;
layout( location = 1 ) rayPayloadEXT bool isShadowed;

layout( binding = 0, set = 0 ) uniform accelerationStructureEXT topLevelAS;

layout( binding = 1, set = 1 ) buffer GeometryInstances
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

layout( binding = 3, set = 2 ) uniform sampler2D textures[];

layout( push_constant ) uniform Constants
{
  vec4 clearColor;
  int frameCount;
  uint sampleRatePerPixel;
  uint maxPathDepth;
  bool useEnvironmentMap;

  uint padding0;
  uint padding1;
  uint padding2;
  uint padding3;
};

Vertex unpackVertex( uint index, uint geometryIndex )
{
  vec4 d0 = vertices[nonuniformEXT( geometryIndex )].v[3 * index + 0];
  vec4 d1 = vertices[nonuniformEXT( geometryIndex )].v[3 * index + 1];
  vec4 d2 = vertices[nonuniformEXT( geometryIndex )].v[3 * index + 2];

  Vertex v;
  v.pos      = d0.xyz;
  v.normal   = vec3( d0.w, d1.x, d1.y );
  v.color    = vec3( d1.z, d1.w, d2.x );
  v.texCoord = vec2( d2.y, d2.z );
  return v;
}

void main( )
{
  uint geometryIndex = geometryInstances.i[gl_InstanceID].geometryIndex;

  ivec3 ind = ivec3( indices[nonuniformEXT( geometryIndex )].i[3 * gl_PrimitiveID + 0],   //
                     indices[nonuniformEXT( geometryIndex )].i[3 * gl_PrimitiveID + 1],   //
                     indices[nonuniformEXT( geometryIndex )].i[3 * gl_PrimitiveID + 2] ); //

  Vertex v0 = unpackVertex( ind.x, geometryIndex );
  Vertex v1 = unpackVertex( ind.y, geometryIndex );
  Vertex v2 = unpackVertex( ind.z, geometryIndex );

  const vec3 barycentrics = vec3( 1.0 - attribs.x - attribs.y, attribs.x, attribs.y );

  // Computing the normal at hit position
  vec3 normal = v0.normal * barycentrics.x + v1.normal * barycentrics.y + v2.normal * barycentrics.z;

  // Transforming the normal to world space
  normal = normalize( vec3( normal * gl_WorldToObjectEXT ) );

  vec3 worldPos = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT;

  vec2 texCoord = v0.texCoord * barycentrics.x + v1.texCoord * barycentrics.y + v2.texCoord * barycentrics.z;

  // Access the meshes SSBO to figure out to which sub-mesh the current triangle belongs and retrieve its material.
  Material mat;
  bool found       = false;
  int subMeshCount = meshes[nonuniformEXT( geometryIndex )].m.length( );

  for ( int i = 0; i < subMeshCount; ++i )
  {
    uint offset     = meshes[nonuniformEXT( geometryIndex )].m[i].indexOffset;
    uint prevOffset = 0;

    if ( i > 0 )
    {
      prevOffset = meshes[nonuniformEXT( geometryIndex )].m[i - 1].indexOffset;
    }

    if ( gl_PrimitiveID < offset && gl_PrimitiveID >= prevOffset )
    {
      mat   = meshes[nonuniformEXT( geometryIndex )].m[i].material;
      found = true;
      break;
    }
  }

  // Colors
  vec3 diffuse  = vec3( 0.0 );
  vec3 emission = vec3( 0.0 ); // emittance / emissiveFactor

  if ( found )
  {
    emission = mat.emission.xyz;

    // Stop recursion if a light source is hit.
    if ( emission != vec3( 0.0 ) )
    {
      ray.depth = maxPathDepth + 1;
    }

    // Retrieve material textures and colors
    diffuse = mat.diffuse.xyz;
    if ( mat.diffuse.w != -1.0 )
    {
      diffuse *= texture( textures[nonuniformEXT( int( mat.diffuse.w ) )], texCoord ).xyz;
    }
  }

  // Pick a random direction from here and keep going.
  vec3 rayOrigin = worldPos;
  vec3 nextDirection;

  // Metallic reflection
  if ( found && mat.illum == 2 )
  {
    const vec3 reflectionDirection = reflect( ray.direction, normal ); // Normal is not correct for sub meshes

    nextDirection  = reflectionDirection + mat.fuzziness * samplingHemisphere( ray.seed, normal );
    ray.reflective = true;
  }
  // @todo Resulting background color is inverted for any 2D surface
  // Dielectric reflection ( Peter Shirley's "Ray Tracing in one Weekend" Chapter 9 )
  else if ( found && mat.illum == 1 )
  {
    vec3 outwardNormal;
    vec3 reflected = reflect( ray.direction, normal );
    float niOverNt;
    vec3 refracted;
    float reflectProb;
    float cosine;

    // Back of triangle - invert normal
    if ( gl_HitKindEXT == gl_HitKindBackFacingTriangleEXT )
    {
      outwardNormal = -normal;
      niOverNt      = mat.ni;
      cosine        = dot( ray.direction, normal ) / ray.direction.length( );
      cosine        = sqrt( 1 - mat.ni * mat.ni * ( 1 - cosine * cosine ) );
    }
    // Front of triangle - take surface normal (world space)
    else
    {
      outwardNormal = normal;
      niOverNt      = 1.0 / mat.ni;
      cosine        = -dot( ray.direction, normal ) / ray.direction.length( );
    }

    if ( refract2( ray.direction, outwardNormal, niOverNt, refracted ) )
    {
      reflectProb = Schlick( cosine, mat.ni );
    }
    else
    {
      reflectProb = 1.0;
    }

    if ( rnd( ray.seed ) < reflectProb )
    {
      nextDirection = normal + samplingUnitSphere( ray.seed );
    }
    else
    {
      nextDirection  = refracted;
      ray.refractive = true;
      ray.reflective = true;
    }

    //diffuse = vec3( 1.0, 1.0, 1.0 );
  }
  // Diffuse reflection
  else if ( found && mat.illum == 0 )
  {
    nextDirection  = samplingHemisphere( ray.seed, normal );
    ray.reflective = false;
  }

  // Probability of the new ray (cosine-distributed)
  const float p = 1 / M_PI;

  // BSDF (Divide by Pi to ensure energy conversation)
  vec3 BSDF = diffuse / M_PI;

  // Assume Lambertian reflection
  float cosTheta = dot( nextDirection, normal );

  ray.origin    = rayOrigin;
  ray.direction = nextDirection;
  ray.emission  = emission;
  ray.weight    = BSDF * cosTheta / p;
}
