#version 420

// Input
layout(location = 0) in vec2 texture_coord;

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
vec2 textureCoord = vec2(texture_coord.x, (flipVertical != 0) ? 1 - texture_coord.y : texture_coord.y); // Flip texture

vec4 getPixel(int i, int j, sampler2D textureSampler, ivec2 size)
{
    //if (i < 0 || j < 0 || i > size.x || j > size.y)
    //    return vec4(0, 0, 0, 0);

    vec2 texelSize = 1.0 / size;

    ivec2 texelCoord = ivec2(textureCoord * vec2(size));
    ivec2 offsetCoord = texelCoord + ivec2(i, j); // Apply the offset

    // Use texelFetch for precise texel access
    return texelFetch(textureSampler, offsetCoord, 0);
}


vec4 grayscale(vec4 color)
{
    float gray = 0.21 * color.r + 0.71 * color.g + 0.07 * color.b; 
    return vec4(gray, gray, gray,  0);
}

vec4 sobel(int x, int y, sampler2D textureSampler, ivec2 size)
{
    // Sobel kernels
    mat3 sobelX = mat3(-1, 0, 1, -2, 0, 2, -1, 0, 1);
    mat3 sobelY = mat3(-1, -2, -1, 0, 0, 0, 1, 2, 1);

    // Initialize gradient values
    vec3 gradientX = vec3(0.0);
    vec3 gradientY = vec3(0.0);

    // Compute gradients using Sobel kernels
    for (int i = x-1; i <= x+1; ++i)
    {
        for (int j = y-1; j <= y+1; ++j)
        {
            // Sample the pixel value
            vec3 pixel = grayscale(getPixel(i, j, textureSampler, size)).xyz;

            // Update gradients
            gradientX += pixel * sobelX[i + 1][j + 1];
            gradientY += pixel * sobelY[i + 1][j + 1];
        }
    }

    // Compute the magnitude of the gradient
    float magnitude = length(gradientX.xy) + length(gradientY.xy);

    if (magnitude <= 0.9) {
        magnitude = 0;
    } else {
        magnitude = 1;
    }

    // Return the gradient as a vec4
    return vec4(vec3(magnitude), 1.0);
}


void main()
{
    //vec4 color = getPixel(0, 0, textureImage, imageSize);

    out_color = sobel(0, 0, textureImage, imageSize);

    /*float count = 0.0f;

    for (int x = 0; x <= markSize.x; x++) {
        for (int y = 0; y < markSize.y; y++) {
           //vec4 imgSobel = sobel(x, y, textureImage, imageSize);
           vec4 markSobel = sobel(x, y, textureMark, markSize);
            if (getPixel(x, y, textureImage, imageSize) == markSobel) {
                count = count + 1;
            }
            
        }
    }

    out_color = vec4(vec3(count / float(markSize.x * markSize.y)), 1);*/
}
