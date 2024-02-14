#version 420

// Input
layout(location = 0) in vec2 texture_coord;

// Uniform properties
uniform sampler2D textureImage;

uniform ivec2 imageSize;

uniform int flipVertical;
uniform int outputMode = 2; // 0: original, 1: grayscale, 2: blur

// Output
layout(location = 0) out vec4 out_color;

// Local variables
vec2 textureCoord = vec2(texture_coord.x, (flipVertical != 0) ? 1 - texture_coord.y : texture_coord.y); // Flip texture

vec4 getPixel(int i, int j, sampler2D textureSampler, ivec2 size)
{
    vec2 texelSize = 1.0 / size;

    // Calculate the adjusted texture coordinates
    vec2 adjustedCoord = textureCoord + vec2(i, j) * texelSize;

    // Check if the adjusted coordinates are within the screen boundaries
    if (adjustedCoord.x < 0.0 || adjustedCoord.x > 1.0 || adjustedCoord.y < 0.0 || adjustedCoord.y > 1.0)
    {
        // Coordinates are out of bounds, return black pixel
        return vec4(0.0, 0.0, 0.0, 1.0);
    }
    else
    {
        // Coordinates are within bounds, sample the texture
        return texture(textureSampler, adjustedCoord);
    }
}

void main()
{
    vec4 color = getPixel(0, 0, textureImage, imageSize);

    out_color = color;
}
