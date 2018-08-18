#version 330 core

out vec4 Color;

in vec3 vColor;
uniform float radius;

void main()
{
    Color = vec4(vColor.x, vColor.y, radius, vColor.z);
}