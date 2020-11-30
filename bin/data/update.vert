#version 410

uniform float delta;
uniform float time;
uniform float life;
uniform float pi = 3.14f;
uniform float force = 60000.f;

layout (location = 0) in vec4 in_position;
layout (location = 5) in vec3 in_velocity;
layout (location = 6) in float in_age;
layout (location = 7) in vec3 in_init_velocity;

out vec4 out_position;
out vec3 out_velocity;
out float out_age;

void main()
{
    if (in_age < 0.f || in_age > life)
    {
        //out_position = vec4(vec3(0.f), 1.f);
        out_position = vec4(50.f * cos(time), 25.f * sin(0.3 * time + pi * 0.5f), 50.f * cos(0.5f * time + pi), 1.f);
        out_velocity = in_init_velocity;

        if (in_age < 0.f)
            out_age = in_age + delta;
        else
            out_age = (in_age - life) + delta;
    }
    else
    {
        vec3 sink = vec3(200.f * sin(time), 250.f * cos(2.f * time + pi * 0.5f), 150.f * sin(3.f * time + pi));
        vec3 dist = sink - in_position.xyz;
        vec3 acc = (force / length(dist)) * normalize(dist);

        out_position = vec4(in_position.xyz + in_velocity * delta + 0.5f * acc * delta * delta, 1.f);
        out_velocity = in_velocity + acc * delta;
        out_age = in_age + delta;
    }
}

