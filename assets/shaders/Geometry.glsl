struct Material
{
  vec4 ambient;  // vec3 ambient  + vec1 texture index
  vec4 diffuse;  // vec3 diffuse  + vec1 texture index
  vec4 specular; // vec3 specular + vec1 texture index
  vec4 emission; // vec3 emission + shininess

  uint illum;
  float d;
  float fuzziness;
  float ni;
};

struct Mesh
{
  Material material;

  uint indexOffset;
  uint padding2;
  uint padding3;
  uint padding4;
};

struct Vertex
{
  vec3 pos;
  vec3 normal;
  vec3 color;
  vec2 texCoord;

  float padding0;
};
