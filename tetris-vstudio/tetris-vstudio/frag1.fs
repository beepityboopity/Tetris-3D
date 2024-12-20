#version 330 core

in vec2 coord;

out vec4 FragColor;

uniform sampler2D currentTexture;
uniform vec3 boxColor;

void main()
{
    FragColor = texture(currentTexture, coord);
    if(FragColor.x == 1.0 && FragColor.y == 1.0) discard;
    else if(FragColor.x != 0.0){
        FragColor = vec4(boxColor, FragColor.w);
    }
}
