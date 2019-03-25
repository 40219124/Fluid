#version 330 core

float maxBlue = 4.0;

in float height;

out vec4 color;

void main()
{
    color = vec4(height / (maxBlue * 4.0), height / (maxBlue * 2.0), height / maxBlue, 1.0);
}