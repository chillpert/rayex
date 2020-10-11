struct DirectionalLight
{
  vec4 ambient;  // vec3 for color + vec1 for intensity
  vec4 diffuse;  // vec3 for color + vec1 for intensity
  vec4 specular; // vec3 for color + vec1 for intensity

  vec4 direction; // vec3 for position + vec1 for bool exists
};

struct PointLight
{
  vec4 ambient;  // vec3 for color + vec1 for intensity
  vec4 diffuse;  // vec3 for color + vec1 for intensity
  vec4 specular; // vec3 for color + vec1 for intensity

  vec4 position; // vec3 for position + vec1 for bool exists

  float constant;
  float linear;
  float quadratic;
};
