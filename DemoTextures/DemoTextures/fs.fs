#version 330 core
out vec4 Color;

in vec3 vColor;
in vec2 vUV;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

void main()
{
    Color = mix(texture(ourTexture, vUV) * vec4(vColor, 0.5), texture(ourTexture2, vUV), 0.2);
   // Color = vec4(vColor, 1.0);
}