#version 460
#extension GL_NV_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable

layout(location = 0) rayPayloadInNV vec3 hitValue;
hitAttributeNV vec3 attribs;

layout(binding = 0, set = 1) buffer Vertices { vec4 v[]; } vertices;
layout(binding = 1, set = 1) buffer Indices { uint i[]; } indices;

void main()
{
  hitValue = vec3(0.2, 0.5, 0.5);
}
