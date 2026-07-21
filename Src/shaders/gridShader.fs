#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec4 slot = texture(texture1,TexCoords);
    vec4 item = texture(texture2,TexCoords);
    FragColor = mix(slot,item,item.a);
}
