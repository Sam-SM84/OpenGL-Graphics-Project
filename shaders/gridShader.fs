#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D slotTexture;
uniform sampler2D itemTexture;

void main()
{
    vec4 slot = texture(slotTexture,TexCoords);
    vec4 item = texture(itemTexture,TexCoords);
    if(distance(item.rgb,vec3(1.0)) < 0.1)
        item.a = 0;
    FragColor = mix(slot,item,item.a);
}