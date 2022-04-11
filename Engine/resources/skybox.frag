#version 330 core
const int LIGHTS_NUMBER = 8; // Note: this value must be the same as in Constants.hpp

struct Light
{
    vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
    vec4 attenuation;
    bool enabled;
};

layout(std140) uniform shaderData
{
    Light lights[LIGHTS_NUMBER];
    vec4 camPos;
};

uniform sampler2D texture;
in vec4 worldPos;
in vec4 normalDir;
in vec2 texCoords;
out vec4 FragColor;

// MAIN
void main()
{
    FragColor = texture2D(texture, texCoords);
}