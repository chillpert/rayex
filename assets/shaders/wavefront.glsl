struct Wavefront
{
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  glm::vec3 transmittance;
  glm::vec3 emission;

  float shininess;
  float reflectionIndex;
  float opacity;
}
