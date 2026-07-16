#version 330 core

out vec4 FragColor;
uniform vec3 Inputcolor;

void main()
{    
    FragColor = vec4(Inputcolor,1.0);
}