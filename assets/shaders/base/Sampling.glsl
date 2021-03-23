#include "Random.glsl"

// @Nvidia vk_ray_tracing_KHR tutorial
// Return the tangent and binormal from the incoming normal
void createCoordinateSystem( in vec3 N, out vec3 Nt, out vec3 Nb )
{
  if ( abs( N.x ) > abs( N.y ) )
  {
    Nt = vec3( N.z, 0, -N.x ) / sqrt( N.x * N.x + N.z * N.z );
  }
  else
  {
    Nt = vec3( 0, -N.z, N.y ) / sqrt( N.y * N.y + N.z * N.z );
  }

  Nb = cross( N, Nt );
}

// From Nvidia's vk_mini_path_tracer and ultimately from Peter Shirley's "Ray Tracing in one Weekend"
// Randomly sampling in hemisphere
// Generates a random point on a sphere of radius 1 centered at the normal. Uses random_unit_vector function
vec3 cosineHemisphereSampling2( inout uint seed, inout float pdf, in vec3 normal )
{
  // PDF of cosine distributed hemisphere sampling
  pdf = 1.0 / M_PI;

  // cosine distributed sampling
  float theta = 2.0 * M_PI * rnd( seed ); // Random in [0, 2pi]
  float u     = 2.0 * rnd( seed ) - 1.0;  // Random in [-1, 1]
  float r     = sqrt( 1.0 - u * u );

  vec3 direction = normal + vec3( r * cos( theta ), r * sin( theta ), u );
  return normalize( direction );
}

vec3 cosineHemisphereSampling( inout uint seed, inout float pdf, in vec3 normal )
{
  // cosine distributed sampling
  float u0 = rnd( seed );
  float u1 = rnd( seed );
  float sq = sqrt( 1.0 - u1 );

  vec3 direction = vec3( cos( 2 * M_PI * u0 ) * sq, sin( 2 * M_PI * u0 ) * sq, sqrt( u1 ) );

  // PDF of cosine distributed hemisphere sampling
  pdf = 1.0 / M_PI;

  // transform to local tangent space
  vec3 tangent;
  vec3 bitangent;
  createCoordinateSystem( normal, tangent, bitangent );

  direction = direction.x * tangent + direction.y * bitangent + direction.z * normal;

  return direction;
}

vec3 uniformSphereSampling( inout uint seed, inout float pdf, in vec3 normal )
{
  pdf = 1.0 / ( 2.0 * M_PI ); // is it though?

  vec3 inUnitSphere = vec3( 0.0 );
  do
  {
    // x,y, and z range from -1 to 1
    // if point is outside the sphere -> reject it and try again
    inUnitSphere = vec3( rnd( seed ), rnd( seed ), rnd( seed ) ) * 2.0 - 1.0;
  } while ( dot( inUnitSphere, inUnitSphere ) >= 1.0 );

  return inUnitSphere;
}

// A rejection method ( "Ray Tracing in one Weekend" p. 22)
vec3 uniformHemisphereSampling( inout uint seed, inout float pdf, in vec3 normal )
{
  vec3 inUnitSphere = uniformSphereSampling( seed, pdf, normal );

  pdf = 1.0 / M_PI; // is it though?

  // In the same hemisphere as the normal
  if ( dot( inUnitSphere, normal ) > 0.0 )
  {
    return inUnitSphere;
  }
  // Flip normal
  else
  {
    return -inUnitSphere;
  }
}

// @https://www.iue.tuwien.ac.at/phd/ertl/node100.html
// Cosine distributed
// not working
vec3 samplingCone2( inout uint seed, inout float pdf, in vec3 normal, in vec3 reflectionDirection )
{
  vec3 dir;
  float a;

  do
  {
    dir = uniformSphereSampling( seed, pdf, normal );
    dir += reflectionDirection;
    a = sqrt( dir.x * dir.x + dir.y * dir.y + dir.z * dir.z );
  } while ( a != 0.0 );

  dir = dir / a;

  return normal + normalize( dir );
}

vec3 samplingCone( inout uint seed, float maxTheta )
{
  float u0 = rnd( seed );
  float u1 = rnd( seed );

  float cosTheta = ( 1.0 - u0 ) + u0 * maxTheta;
  float sinTheta = sqrt( 1 - cosTheta * cosTheta );
  float phi      = u1 * 2.0 * M_PI;

  vec3 dir;
  dir.x = cos( phi ) * sinTheta;
  dir.y = sin( phi ) * sinTheta;
  dir.z = cosTheta;

  return normalize( dir );
}
