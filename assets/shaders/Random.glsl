#define M_PI 3.141592

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

// Generate a random unsigned int in [0, 2^24) given the previous RNG state
// using the Numerical Recipes linear congruential generator
uint lcg( inout uint prev )
{
  uint LCG_A = 1664525u;
  uint LCG_C = 1013904223u;
  prev       = ( LCG_A * prev + LCG_C );
  return prev & 0x00FFFFFF;
}

// Generate a random float in [0, 1) given the previous RNG state
float rnd( inout uint prev )
{
  return ( float( lcg( prev ) ) / float( 0x01000000 ) );
}

// Randomly sampling around +Z
vec3 samplingHemisphere( inout uint seed, in vec3 tangent, in vec3 bitangent, in vec3 normal )
{
  float u0 = clamp( rnd( seed ), 0.0, 3.0 );
  float u1 = clamp( rnd( seed ), 0.0, 3.0 );
  float sq = sqrt( u1 );

  vec3 direction;

  // Sample direction.
  direction.x = ( cos( 2 * M_PI * u0 ) * sq );
  direction.y = ( sin( 2 * M_PI * u0 ) * sq );
  direction.z = sqrt( 1.0 - u1 );

  // Transform to local tangent space of the surface.
  direction = direction.x * tangent + direction.y * bitangent + direction.z * normal;

  return direction;
}

// Return the tangent and binormal from the incoming normal
void createCoordinateSystem( in vec3 normal, out vec3 tangent, out vec3 bitangent )
{
  if ( abs( normal.x ) > abs( normal.y ) )
  {
    tangent = vec3( normal.z, 0, -normal.x ) / sqrt( normal.x * normal.x + normal.z * normal.z );
  }
  else
  {
    tangent = vec3( 0, -normal.z, normal.y ) / sqrt( normal.y * normal.y + normal.z * normal.z );
  }

  bitangent = cross( normal, tangent );
}
