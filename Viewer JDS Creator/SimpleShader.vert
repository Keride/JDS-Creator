#version 330 core

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

layout (location = 0) in vec3 in_Position;
uniform vec4 Color;

out vec4 pass_Color;

void main(void)
{
     gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
     pass_Color = Color;
}