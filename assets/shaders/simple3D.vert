#version 460
#extension GL_ARB_separate_shader_objects : enable

// IN
layout(binding = 0, set = 0) uniform CameraProperties
{
  mat4 view;
  mat4 proj;
  mat4 viewInverse;
  mat4 projInverse;
  vec3 cameraPos;
} cam;

struct RayTracingInstance
{
  mat4 transform;
  mat4 transformIT;
  int modelIndex;
  int txtOffset;

  int nodeId; // ignore
  float padding1;
};

layout(binding = 2, set = 0) buffer RayTracingInstances
{
  RayTracingInstance i[];
} rayTracingInstances;

// gl_InstanceIndex can be used just like gl_InstanceID in rchit. i guess.

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inTexCoord;

// OUT
layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out vec3 cameraPos;

layout(push_constant) uniform Constants
{
  int index;
};

void main()
{
  mat4 model = rayTracingInstances.i[index].transform;

  fragPos = vec3(model * vec4(inPosition, 1.0));
  fragTexCoord = inTexCoord;
  fragNormal = normalize(mat3(transpose(inverse(model))) * inNormal);
  cameraPos = cam.cameraPos;

  gl_Position = cam.proj * cam.view * model * vec4(inPosition, 1.0);
}