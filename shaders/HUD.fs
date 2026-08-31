#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform vec3 color;

void main()
{    
    vec4 texColor = texture(texture1, TexCoords);
    if(texColor.r > 0.95 && texColor.g > 0.95 && texColor.b > 0.95)
        discard;
    FragColor = vec4(color,texColor.a);
}
