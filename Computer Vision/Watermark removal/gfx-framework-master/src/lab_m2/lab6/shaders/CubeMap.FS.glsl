#version 430

// Input
layout(location = 0) in vec3 world_position;
layout(location = 1) in vec3 world_normal;

// Uniform properties
uniform samplerCube texture_cubemap;
uniform int type;

uniform vec3 camera_position;

// Output
layout(location = 0) out vec4 out_color;

vec3 myReflect()
{
    // Compute reflection vector
    vec3 viewDir = world_position - camera_position;
    vec3 reflectionDir = reflect(viewDir, world_normal);

    // Sample from the cubemap using the reflection vector
    return texture(texture_cubemap, reflectionDir).rgb;
}

vec3 myRefract(float refractive_index)
{
    // Compute refraction vector
    vec3 viewDir = normalize(world_position - camera_position);
    vec3 refractionDir = refract(viewDir, normalize(world_normal), refractive_index);

    // Sample from the cubemap using the refraction vector
    return texture(texture_cubemap, refractionDir).rgb;
}

void main()
{
    if (type == 0)
    {
        out_color = vec4(myReflect(), 1.0);
    }
    else
    {
        out_color = vec4(myRefract(1.33), 1.0);
    }
}