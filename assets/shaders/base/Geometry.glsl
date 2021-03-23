struct Material
{
  vec4 diffuse;  // vec3 diffuse  + vec1 texture index
  vec4 emission; // vec3 emission  + vec1 shininess
  uint illum;
  float d;
  float fuzziness;
  float ni;
};

struct Vertex
{
  vec3 pos;
  vec3 normal;
  vec3 color;
  vec2 texCoord;

  float padding0;
};

struct GeometryInstance
{
  mat4 transform;
  mat4 transformIT;
  uint geometryIndex;

  float padding0;
  float padding1;
  float padding2;
};
