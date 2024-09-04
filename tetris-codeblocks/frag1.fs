#version 330 core

in vec2 coord;

out vec4 FragColor;

uniform sampler2D currentTexture;

void main()
{
    FragColor = texture(currentTexture, coord);
    if(FragColor.x == 1.0) discard;
}
