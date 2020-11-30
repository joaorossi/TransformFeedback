#version 410

uniform mat4 modelViewProjectionMatrix;

layout (location = 0) in vec4 position;
layout (location = 6) in float in_age;

out float out_age;

void main()
{
    gl_Position     = modelViewProjectionMatrix * position;
    gl_PointSize    = 3.f;
    out_age = in_age;
}

