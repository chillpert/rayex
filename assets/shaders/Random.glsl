#define M_PI 3.141592

// @Nvidia vk_ray_tracing_KHR tutorial
// Generate a random unsigned int from two unsigned int values, using 16 pairs
// of rounds of the Tiny Encryption Algorithm. See Zafar, Olano, and Curtis,
// "GPU Random Numbers via the Tiny Encryption Algorithm"
uint tea( uint val0, uint val1 )
{
  uint v0 = val0;
  uint v1 = val1;
  uint s0 = 0;

  for ( uint n = 0; n < 16; n++ )
  {
    s0 += 0x9e3779b9;
    v0 += ( ( v1 << 4 ) + 0xa341316c ) ^ ( v1 + s0 ) ^ ( ( v1 >> 5 ) + 0xc8013ea4 );
    v1 += ( ( v0 << 4 ) + 0xad90777d ) ^ ( v0 + s0 ) ^ ( ( v0 >> 5 ) + 0x7e95761e );
  }

  return v0;
}

// @Nvidia vk_ray_tracing_KHR tutorial
// Generate a random unsigned int in [0, 2^24) given the previous RNG state
// using the Numerical Recipes linear congruential generator
uint lcg( inout uint prev )
{
  uint LCG_A = 1664525u;
  uint LCG_C = 1013904223u;
  prev       = ( LCG_A * prev + LCG_C );
  return prev & 0x00FFFFFF;
}

// @Nvidia vk_ray_tracing_KHR tutorial
// Generate a random float in [0, 1) given the previous RNG state
float rnd( inout uint prev )
{
  return ( float( lcg( prev ) ) / float( 0x01000000 ) );
}

// Random functions from Alan Wolfe's excellent tutorials (https://blog.demofox.org/)
uint wang_hash( inout uint seed )
{
  seed = uint( seed ^ uint( 61 ) ) ^ uint( seed >> uint( 16 ) );
  seed *= uint( 9 );
  seed = seed ^ ( seed >> 4 );
  seed *= uint( 0x27d4eb2d );
  seed = seed ^ ( seed >> 15 );
  return seed;
}

float RandomFloat01( inout uint state )
{
  return float( wang_hash( state ) ) / 4294967296.0;
}

// From Nvidia's vk_mini_path_tracer and ultimately from Peter Shirley's "Ray Tracing in one Weekend"
// Randomly sampling in hemisphere
// Generates a random point on a sphere of radius 1 centered at the normal. Uses random_unit_vector function
vec3 samplingHemisphere( inout uint seed, in vec3 normal )
{
  // @todo check performance of both methods

  //// reduce fireflies
  //// but results in loss of energy
  //float u0 = clamp( rnd( seed ), 0.0, 3.0 );
  //float u1 = clamp( rnd( seed ), 0.0, 3.0 );
  //
  //const float theta = M_PI * 2 * u0;  // Random in [0, 2pi]
  //const float u     = 2.0 * u1 - 1.0; // Random in [-1, 1]
  //const float r     = sqrt( 1.0 - u * u );
  //vec3 rayDirection = normal + vec3( r * cos( theta ), r * sin( theta ), u );
  //
  //return normalize( rayDirection );

  // A rejection method ( "Ray Tracing in one Weekend" p. 22)
  vec3 inUnitSphere = vec3( 0.0 );
  do
  {
    // x,y, and z range from -1 to 1
    // if point is outside the sphere -> reject it and try again
    inUnitSphere = vec3( rnd( seed ), rnd( seed ), rnd( seed ) ) * 2.0 - 1.0;
  } while ( dot( inUnitSphere, inUnitSphere ) >= 1.0 );

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

  // cosine distributed from rtgems p. 211
  //float u0 = clamp( rnd( seed ), 0.0, 3.0 );
  //float u1 = clamp( rnd( seed ), 0.0, 3.0 );
  //
  //vec3 dir;
  //dir.x = sqrt( u0 ) * cos( 2.0 * M_PI * u1 );
  //dir.y = sqrt( u0 ) * sin( 2.0 * M_PI * u1 );
  //dir.z = sqrt( 1.0 - u0 );
  //
  //return normal + normalize( dir ); // probably wrong
}

// From raytracinggems p.240
vec3 samplingHemisphere2( inout uint seed )
{
  float u0 = clamp( rnd( seed ), 0.0, 3.0 );
  float u1 = clamp( rnd( seed ), 0.0, 3.0 );

  vec3 dir;
  dir.x = sqrt( u0 ) * cos( 2.0 * M_PI * u1 );
  dir.y = sqrt( u0 ) * sin( 2.0 * M_PI * u1 );
  dir.z = sqrt( 1.0 - u0 );

  return dir;
}

// @https://www.iue.tuwien.ac.at/phd/ertl/node100.html
vec3 samplingUnitSphere2( inout uint seed )
{
  float a;
  float a1;
  float a2;

  do
  {
    a1 = 2.0 * rnd( seed ) - 1;
    a2 = 2.0 * rnd( seed ) - 1;
    a  = a1 * a1 + a2 + a2;
  } while ( a < 1 );

  vec3 dir;
  dir.x = 1.0 - 2.0 * a;
  a     = 2 * sqrt( 1.0 - a );
  dir.y = a1 * a;
  dir.z = a2 * a;

  return normalize( dir );
}

// @https://www.iue.tuwien.ac.at/phd/ertl/node100.html
// Cosine distributed
// not working
vec3 samplingCone2( inout uint seed, vec3 v )
{
  vec3 dir;
  float a;

  do
  {
    dir = samplingUnitSphere2( seed );
    dir += v;
    a = sqrt( dir.x * dir.x + dir.y * dir.y + dir.z * dir.z );
  } while ( a != 0.0 );

  dir = dir / a;
  return normalize( dir );
}

vec3 samplingCone( inout uint seed, float fuzziness )
{
  float u0 = rnd( seed );
  float u1 = rnd( seed );

  float cosTheta = ( 1.0 - u0 ) + u0 * fuzziness;
  float sinTheta = sqrt( 1 - cosTheta * cosTheta );
  float phi      = u1 * 2.0 * M_PI;

  vec3 dir;
  dir.x = cos( phi ) * sinTheta;
  dir.y = sin( phi ) * sinTheta;
  dir.z = cosTheta;

  return normalize( dir );
}

// Randomly sampling in hemisphere ( Peter Shirley's "Ray Tracing in one Weekend" )
vec3 samplingUnitSphere( inout uint seed )
{
  // @todo consider adding clamping here as well

  vec3 pos = vec3( 0.0 );
  do {
    float u0 = rnd( seed ); //clamp( rnd( seed ), 0.0, 1.0 );
    float u1 = rnd( seed ); //clamp( rnd( seed ), 0.0, 1.0 );
    float u2 = rnd( seed ); //clamp( rnd( seed ), 0.0, 1.0 );

    pos = vec3( u0, u1, u2 ) * 2.0 - 1.0;
  } while ( dot( pos, pos ) >= 1.0 );

  return pos;
}

// Randomly sampling in hemisphere ( Peter Shirley's "Ray Tracing in one Weekend" )
float Schlick( const float cosine, const float ior )
{
  float r0 = ( 1.0 - ior ) / ( 1.0 + ior );
  r0 *= r0;
  return r0 + ( 1.0 - r0 ) * pow( 1.0 - cosine, 5.0 );
}

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
