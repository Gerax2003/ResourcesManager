#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uvCoords;
layout (location = 2) in vec3 normals;

uniform mat4 modelMat;
uniform mat4 viewProj;

out vec2 texCoords;

void main()
{
   texCoords = uvCoords;
   gl_Position = viewProj * modelMat * vec4(pos.x, pos.y, pos.z, 1.0);
}