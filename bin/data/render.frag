#version 410

uniform sampler2D tex0;
uniform float life;

in float out_age;

out vec4 outputColor;

void main()
{
    outputColor = texture(tex0, gl_PointCoord) * vec4(1.f, 1.f, 1.f, 1.f - (out_age / life));
}

