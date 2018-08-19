#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aUV;

out vec3 vColor;
out vec2 vUV;

uniform float x;
uniform float y;

void main()
{
	gl_Position = vec4(aPos.x + x - 0.5, (aPos.y + y - 0.5), 0.0, 1.0);
	vColor = aColor;
	vUV = aUV;
}