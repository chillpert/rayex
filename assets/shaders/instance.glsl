struct GeometryInstance
{
  mat4 transform;
  mat4 transformIT;
  uint modelIndex;

  float padding0;
  float padding1;
  float padding2;
};

struct Material
{
  vec4 ambient;  // vec3 ambient  + vec1 texture index
  vec4 diffuse;  // vec3 diffuse  + vec1 texture index
  vec4 specular; // vec3 specular + vec1 texture index
};

struct Mesh
{
  Material material;

  vec4 padding0;
  vec4 padding1;
  vec4 padding2;
  uint padding3;
  uint padding4;
  uint padding5;

  uint indexOffset;
};
