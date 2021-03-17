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
  vec3 localNormal = v0.normal * barycentrics.x + v1.normal * barycentrics.y + v2.normal * barycentrics.z;

  // Transforming the normal to world space
  vec3 normal = normalize( vec3( localNormal * gl_WorldToObjectEXT ) );

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
  vec3 reflectance = vec3( 0.0 );
  vec3 emission    = vec3( 0.0 ); // emittance / emissiveFactor

  if ( found )
  {
    emission = mat.emission.xyz;

    // Stop recursion if a light source is hit.
    if ( emission != vec3( 0.0 ) )
    {
      ray.depth = maxPathDepth + 1;
    }

    // Retrieve material textures and colors
    reflectance = mat.diffuse.xyz;
    if ( mat.diffuse.w != -1.0 )
    {
      reflectance *= texture( textures[nonuniformEXT( int( mat.diffuse.w ) )], texCoord ).xyz;
    }
  }

  // Pick a random direction from here and keep going.
  vec3 rayOrigin = worldPos;
  vec3 nextDirection;

  // Probability of the new ray (PDF)
  float pdf      = 1.0;
  float cosTheta = 1.0;

  // BSDF (Divide by Pi to ensure energy conversation)
  // @todo path regularization: blur the bsdf for indirect rays (raytracinggems p.251)
  vec3 bsdf = reflectance / M_PI;

  // Metallic reflection
  if ( found && mat.illum == 2 )
  {
    const vec3 reflectionDirection = reflect( ray.direction, normal ); // Normal is not correct for sub meshes
    nextDirection                  = reflectionDirection + mat.fuzziness * samplingHemisphere( ray.seed, normal );
    ray.reflective                 = true;

    //p = 1.0 / ( 2.0 * M_PI * ( 1.0 - cos( mat.fuzziness ) ) );
    pdf = 1 / M_PI;

    // If removed than mirror won't be visible at all in mirror scene
    cosTheta = dot( nextDirection, normal ); // The steeper the incident direction to the surface is the more important the sample gets
    bsdf *= cosTheta;
  }
  // @todo Resulting background color is inverted for any 2D surface
  // Dielectric reflection ( Peter Shirley's "Ray Tracing in one Weekend" Chapter 9 )
  else if ( found && mat.illum == 1 )
  {
    // True, if the incoming ray traversed a medium. False, if the incoming ray was in vacuum.
    bool isTransmitted = ( gl_HitKindEXT == gl_HitKindBackFacingTriangleEXT );

    // This is the back of the triangle. This means that a surface has been traversed meaning the incoming ray was refracted.
    // So let's refract again!
    ray.refractive = isTransmitted ? true : false;

    // Flip the normal if transmitted
    vec3 temp = isTransmitted ? -normal : normal;

    float dot    = dot( ray.direction, normal );
    float cosine = dot > 0 ? mat.ni * dot : -dot;
    float ior    = dot > 0 ? mat.ni : 1 / mat.ni;

    vec3 refracted    = refract( ray.direction, temp, ior );
    float reflectProb = refracted != vec3( 0.0 ) ? Schlick( cosine, mat.ni ) : 1.0;

    if ( rnd( ray.seed ) < reflectProb && !ray.refractive )
    {
      const vec3 reflectionDirection = reflect( ray.direction, normal ); // Normal is not correct for sub meshes
      nextDirection                  = reflectionDirection + mat.fuzziness * samplingHemisphere( ray.seed, normal );
      ray.reflective                 = true;
    }
    else
    {
      nextDirection = refracted;
    }

    pdf = 1.0 / M_PI;
  }
  // Diffuse / Lambertian reflection
  else if ( found && mat.illum == 0 )
  {
    nextDirection = samplingHemisphere( ray.seed, normal );
    pdf           = 1.0 / M_PI;                   // PDF of sampling a hemisphere
    cosTheta      = dot( nextDirection, normal ); // The steeper the incident direction to the surface is the more important the sample gets
    bsdf *= cosTheta;
  }

  pdf = 1.0 / ( 1.5 * M_PI ); // Overwrites all other PDFs. This is completely random and probably wrong!

  ray.origin    = rayOrigin;
  ray.direction = nextDirection;
  ray.emission  = emission;
  ray.weight    = bsdf / pdf; // divide reflectance by PDF
}
