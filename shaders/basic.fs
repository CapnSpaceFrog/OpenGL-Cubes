#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D mainTexture;
uniform sampler2D subTextureOne;

void main()
{
   FragColor = texture(mainTexture, TexCoord);
}