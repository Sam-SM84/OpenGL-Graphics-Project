#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec4 slot = texture(texture1,TexCoords);
    vec4 item = texture(texture2,TexCoords);
    if(distance(item.rgb,vec3(1.0)) < 0.1)
        item.a = 0;
    FragColor = mix(slot,item,item.a);
}
