#version 330 core

in vec3 fragCol;

out vec4 FragColor;

void main()
{
   FragColor = vec4(fragCol, 1.0);
}
