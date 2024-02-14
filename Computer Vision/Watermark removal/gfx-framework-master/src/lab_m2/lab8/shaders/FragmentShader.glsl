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

vec4 grayscale(vec4 color)
{
    float gray = 0.21 * color.r + 0.71 * color.g + 0.07 * color.b; 
    return vec4(gray, gray, gray,  0);
}


vec4 blur(int radius, vec2 offset)
{
    vec2 texelSize = 1.0f / screenSize;
    vec4 sum = vec4(0);
    vec2 coord = textureCoord + offset;
    for(int i = -radius; i <= radius; i++)
    {
        for(int j = -radius; j <= radius; j++)
        {
            sum += texture(textureImage, coord + vec2(i, j) * texelSize);
        }
    }
        
    float samples = pow(2 * radius + 1, 2);
    return sum / samples;
}

vec4 median(int radius)
{
    vec2 texelSize = 1.0 / screenSize;
    int kernelSize = (2 * radius + 1) * (2 * radius + 1);

    vec4 values[100];

    int index = 0;
    for (int i = -radius; i <= radius; ++i)
    {
        for (int j = -radius; j <= radius; ++j)
        {
            vec4 pixel = texture(textureImage, textureCoord + vec2(i, j) * texelSize);
            values[index] = pixel;
            index++;
        }
    }

    for (int i = 0; i < kernelSize - 1; ++i)
    {
        for (int j = 0; j < kernelSize - i - 1; ++j)
        {
            if (values[j].r > values[j + 1].r)
            {
                vec4 temp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = temp;
            }
        }
    }
    
    return values[kernelSize / 2];
}

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
            //vec3 pixel = texture(textureImage, textureCoord + vec2(i, j) * texelSize).xyz;
            vec3 pixel = grayscale(texture(textureImage, textureCoord + vec2(i, j) * texelSize)).xyz;

            // Update gradients
            gradientX += pixel * sobelX[i + 1][j + 1];
            gradientY += pixel * sobelY[i + 1][j + 1];
        }
    }

    // Compute the magnitude of the gradient
    float magnitude = length(gradientX.xy) + length(gradientY.xy);

    // Thresholding
    if (magnitude <= 0.5) {
        magnitude = 0;
    } else {
        magnitude = 1;
    }

    // Return the gradient as a vec4
    return vec4(vec3(magnitude), 1.0);
}


void main()
{
    out_color = sobel();
}
