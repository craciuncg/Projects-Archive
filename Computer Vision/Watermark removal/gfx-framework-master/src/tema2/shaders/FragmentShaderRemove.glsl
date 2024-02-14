#version 420

// Input
layout(location = 0) in vec2 texture_coord_image;

// Uniform properties
uniform sampler2D textureImage;
uniform sampler2D textureMark;
uniform sampler2D textureDetect;

uniform ivec2 imageSize;
uniform ivec2 markSize;

uniform int flipVertical;
uniform int outputMode = 2; // 0: original, 1: grayscale, 2: blur

// Output
layout(location = 0) out vec4 out_color;

// Local variables

vec4 getPixel(int i, int j, vec2 texture_coord, sampler2D textureSampler, ivec2 size, int flip)
{
    vec2 texelSize = 1.0 / size;
    vec2 textureCoord = vec2(texture_coord.x, (flip != 0) ? 1 - texture_coord.y : texture_coord.y); // Flip texture

    ivec2 texelCoord = ivec2(textureCoord * vec2(size));
    ivec2 offsetCoord = texelCoord + ivec2(i, j); // Apply the offset

    // Use texelFetch for precise texel access
    return texelFetch(textureSampler, offsetCoord, 0);
}

void main()
{
    vec4 color = getPixel(0, 0, texture_coord_image, textureImage, imageSize, 1);

    ivec2 size = markSize;

    for (int y = 0; y >= -size.y; y--) 
    {
        for (int x = 0; x >= -size.x; x--)
        {
            vec4 detected = getPixel(x, y + 1, texture_coord_image, textureDetect, imageSize, 0);
            if (detected.x == 1)
            {
                vec4 markColor = getPixel(-x, size.y + y, vec2(0.0f, 0.0f), textureMark, size, 0);

                /*if (color.r == 1 && markColor.r != 1)
                {
                    out_color = vec4(1, 0, 1, 1);
                    return;
                }

                if (detected.r == 1 && (x == 0 || y == 0)) {
                    out_color = vec4(0, 1, 1, 1);
                    return;
                }*/

                /*if ((x == 0 || y == 0 || x == -size.x + 1|| y == -size.y + 1) && detected.x == 1) {
                    out_color = vec4(1, 0, 0, 1);
                    return;
                }*/

                

                out_color = color - markColor;
                return;
            }
        }
    }
    
    out_color = color;
}
