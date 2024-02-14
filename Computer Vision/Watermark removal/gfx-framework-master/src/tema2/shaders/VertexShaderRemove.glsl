#version 410

// Input
layout(location = 0) in vec3 v_position;
layout(location = 2) in vec2 v_texture_coord;

// Output
layout(location = 0) out vec2 texture_coord_image;

void main()
{
    texture_coord_image = v_texture_coord;
    gl_Position = vec4(v_position, 1.0);
}
