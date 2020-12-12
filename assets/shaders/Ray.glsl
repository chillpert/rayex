struct RayPayLoad
{
  vec3 rayDirection;
  vec3 hitValue;
  vec3 rayOrigin;
  vec3 weight;
  vec3 attenuation;
  uint seed;
  uint depth;
  float reflectivity;
};
