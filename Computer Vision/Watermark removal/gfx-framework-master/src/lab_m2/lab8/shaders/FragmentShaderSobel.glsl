#version 410

// Input
layout(location = 0) in vec2 texture_coord;

// Uniform properties
uniform sampler2D textureImage;
uniform ivec2 screenSize;
uniform int flipVertical;
uniform int outputMode = 2; // 0: original, 1: grayscale, 2: blur

// Output
layout(location = 0) out vec4 out_color;

// Local variables
vec2 textureCoord = vec2(texture_coord.x, (flipVertical != 0) ? 1 - texture_coord.y : texture_coord.y); // Flip texture

vec4 sobel()
{
    vec2 texelSize = 1.0 / screenSize;

    // Sobel kernels
    mat3 sobelX = mat3(-1, 0, 1, -2, 0, 2, -1, 0, 1);
    mat3 sobelY = mat3(-1, -2, -1, 0, 0, 0, 1, 2, 1);

    // Initialize gradient values
    vec3 gradientX = vec3(0.0);
    vec3 gradientY = vec3(0.0);

    // Compute gradients using Sobel kernels
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            // Sample the pixel value
            vec3 pixel = texture(textureImage, textureCoord + vec2(i, j) * texelSize).xyz;

            // Update gradients
            gradientX += pixel * sobelX[i + 1][j + 1];
            gradientY += pixel * sobelY[i + 1][j + 1];
        }
    }

    // Compute the magnitude of the gradient
    float magnitude = length(gradientX.xy) + length(gradientY.xy);

    // Return the gradient as a vec4
    return vec4(vec3(magnitude), 1.0);
}


void main()
{
    out_color = vec4(1, 0, 0, 1);
}
