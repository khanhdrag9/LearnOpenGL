#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vColor;
uniform float xp;

void main()
{
	gl_Position = vec4(aPos.x - 0.5 + xp, aPos.y, 0.0, 1.0);
	vColor = aColor;
}