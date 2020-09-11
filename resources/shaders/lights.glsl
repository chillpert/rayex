struct DirectionalLight
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float ambientStrength;

  vec3 direction;
  float exists;
};

struct PointLight
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float ambientStrength;

  vec3 position;

  float constant;
  float linear;
  float quadratic;
};

struct SpotLight
{
  vec3 position;
  vec3 direction;

  float cutOff;
  float outerCutOff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float ambientStrength;

  float constant;
  float linear;
  float quadratic;
};