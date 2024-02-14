#version 420

// Input
layout(location = 0) in vec2 texture_coord_image;

// Uniform properties
uniform sampler2D textureImage;
uniform sampler2D textureMark;

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
    float count = 0.0f;
    float numWhiteMark = 0.0f;

    for (int y = 0; y <= markSize.y; y++) 
    {
        for (int x = 0; x <= markSize.x; x++)
        {
            vec4 colorImage = getPixel(x, y, texture_coord_image, textureImage, imageSize, 0);
            vec4 colorMark = getPixel(x, y, vec2(0, 0), textureMark, markSize, 0);

            if (colorMark.x == 1)
            {
                numWhiteMark += 1.0f;

                if (colorImage.x == 1)
                {
                    count += 1.0f;
                }
            }
        }
    }

    float value = count / numWhiteMark;

    if (value >= 0.95)
    {
        value = 1;
    }
    else
    {
        value = 0;
    }
    
    out_color = vec4(vec3(value), 1);
    //out_color = getPixel(0, 0, textureImage, imageSize);
}
