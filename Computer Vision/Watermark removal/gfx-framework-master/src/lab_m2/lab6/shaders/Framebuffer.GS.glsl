#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 viewMatrices[6];

in vec3 geom_position[3];
in vec2 geom_texture_coord[3];

out vec3 frag_position;
out vec2 frag_texture_coord;

void main()
{
    int i, layer;

    for (layer = 0; layer < 6; ++layer) {
        for (i = 0; i < gl_in.length(); i++) {
            // Transform the position using the corresponding view matrix
            frag_position = (viewMatrices[layer] * vec4(geom_position[i], 1.0)).xyz;
            frag_texture_coord = geom_texture_coord[i];

            // Set gl_Layer to specify the target layer (cubemap face)
            gl_Layer = layer;

            // Transform the vertex position to clip space
            gl_Position = Projection * View * viewMatrices[layer] * gl_in[i].gl_Position;

            EmitVertex();
        }
        EndPrimitive();
    }
}