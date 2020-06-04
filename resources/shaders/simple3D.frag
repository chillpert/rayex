#version 450
// necessary for vulkan shaders
#extension GL_ARB_separate_shader_objects : enable

// IN
layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 cameraPos;

// OUT
layout(location = 0) out vec4 outColor;

vec3 calcDirLight(vec3 normal, vec3 fragPos, vec3 viewDir)
{
  // ambient
  vec3 ambient = 0.5 * vec3(texture(texSampler, fragTexCoord));

  // diffuse
  vec3 lightDir = normalize(-vec3(0.0, -10.0, -10.0));
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diff * vec3(texture(texSampler, fragTexCoord));

  return (ambient + diffuse);
}

void main()
{
  vec3 norm = fragNormal;
  vec3 fragPos = fragPos;
  vec3 viewDir = normalize(cameraPos - fragPos);

  outColor = vec4(calcDirLight(norm, fragPos, viewDir), 1.0);
}