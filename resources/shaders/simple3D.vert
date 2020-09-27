#version 450
#extension GL_ARB_separate_shader_objects : enable

// IN
layout(binding = 0) uniform UniformBufferObject
{
  mat4 model;
  mat4 view;
  mat4 proj;
  vec3 cameraPos;
} ubo;

struct RayTracingInstance
{
  mat4 transform;
  mat4 transformIT;
  int modelIndex;
  int txtOffset;

  float padding0;
  float padding1;
};

//layout(binding = 1, set = 2) buffer RayTracingInstances
//{
//  RayTracingInstance i[];
//} rayTracingInstances;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inTexCoord;

// OUT
layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out vec3 cameraPos;

void main()
{
  fragPos = vec3(ubo.model * vec4(inPosition, 1.0));
  fragTexCoord = inTexCoord;
  fragNormal = normalize(mat3(transpose(inverse(ubo.model))) * inNormal);
  cameraPos = ubo.cameraPos;

  gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
}