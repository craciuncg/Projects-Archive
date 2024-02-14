#include "tema2/tema2.h"

#include <vector>
#include <iostream>

#include "pfd/portable-file-dialogs.h"

using namespace std;
using namespace tema;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
    outputMode = 3;
    gpuProcessing = false;
    saveScreenToImage = false;
    window->SetSize(600, 600);
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    // Load default texture fore imagine processing
    originalImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, "src", "tema2", "star.png"), nullptr, "image", true, true);
    processedImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "cube", "pos_x.png"), nullptr, "newImage", true, true);

    watermarkImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, "src", "tema2", "watermark.png"), nullptr, "watermark", true, true);

    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    std::string shaderPath = PATH_JOIN(window->props.selfDir, "src", "tema2", "shaders");

    // Create a shader program for particle system
    {
        Shader *shader = new Shader("Simple");
        shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader.glsl"), GL_FRAGMENT_SHADER);

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ImageSobel");
        shader->AddShader(PATH_JOIN(shaderPath, "VertexShaderSobel.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "FragmentShaderSobel.glsl"), GL_FRAGMENT_SHADER);

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ImageDetect");
        shader->AddShader(PATH_JOIN(shaderPath, "VertexShaderDetect.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "FragmentShaderDetect.glsl"), GL_FRAGMENT_SHADER);

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ImageRemove");
        shader->AddShader(PATH_JOIN(shaderPath, "VertexShaderRemove.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "FragmentShaderRemove.glsl"), GL_FRAGMENT_SHADER);

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    InitFramebuffer(glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight()), &sobelImage, &fbsobelImage);
    InitFramebuffer(glm::ivec2(watermarkImage->GetWidth(), watermarkImage->GetHeight()), &sobelMark, &fbsobelMark);
    InitFramebuffer(glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight()), &detected, &fbdetected);
    InitFramebuffer(glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight()), &removed, &fbremoved);

}

void Tema2::InitFramebuffer(glm::ivec2 size, GLuint* resultTexture, GLuint* framebuffer)
{
    glGenFramebuffers(1, framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);

    // Init framebuffer
    glGenTextures(1, resultTexture);
    glBindTexture(GL_TEXTURE_2D, *resultTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *resultTexture, 0);

    // Check for framebuffer completeness (optional but recommended)
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        cout << "framebuffer sobel error\n";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Tema2::FrameStart()
{
}

void Tema2::Sobel(GLuint framebufferSobel, GLuint image, glm::ivec2 size)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferSobel);
    glViewport(0, 0, size.x, size.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto shader = shaders["ImageSobel"];
    shader->Use();

    int flip_loc = shader->GetUniformLocation("flipVertical");
    glUniform1i(flip_loc, 1);

    int screenSize_loc = shader->GetUniformLocation("imageSize");
    glUniform2i(screenSize_loc, size.x, size.y);

    int outputMode_loc = shader->GetUniformLocation("outputMode");
    glUniform1i(outputMode_loc, outputMode);

    int locTexture = shader->GetUniformLocation("textureImage");
    glUniform1i(locTexture, 0);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, image);

    RenderMesh(meshes["quad"], shader, glm::mat4(1));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Tema2::Detect()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbdetected);
    glViewport(0, 0, originalImage->GetWidth(), originalImage->GetHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto shader = shaders["ImageDetect"];
    shader->Use();

    int flip_loc = shader->GetUniformLocation("flipVertical");
    glUniform1i(flip_loc, 0);

    int screenSize_loc = shader->GetUniformLocation("imageSize");
    glUniform2i(screenSize_loc, originalImage->GetWidth(), originalImage->GetHeight());

    int markSizeloc = shader->GetUniformLocation("markSize");
    glUniform2i(markSizeloc, watermarkImage->GetWidth(), watermarkImage->GetHeight());

    int outputMode_loc = shader->GetUniformLocation("outputMode");
    glUniform1i(outputMode_loc, outputMode);

    int locTexture = shader->GetUniformLocation("textureImage");
    glUniform1i(locTexture, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sobelImage);

    locTexture = shader->GetUniformLocation("textureMark");
    glUniform1i(locTexture, 1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, sobelMark);

    RenderMesh(meshes["quad"], shader, glm::mat4(1));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Tema2::Remove()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbremoved);
    glViewport(0, 0, originalImage->GetWidth(), originalImage->GetHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto shader = shaders["ImageRemove"];
    shader->Use();

    int flip_loc = shader->GetUniformLocation("flipVertical");
    glUniform1i(flip_loc, 0);

    int screenSize_loc = shader->GetUniformLocation("imageSize");
    glUniform2i(screenSize_loc, originalImage->GetWidth(), originalImage->GetHeight());

    int markSizeloc = shader->GetUniformLocation("markSize");
    glUniform2i(markSizeloc, watermarkImage->GetWidth(), watermarkImage->GetHeight());

    int outputMode_loc = shader->GetUniformLocation("outputMode");
    glUniform1i(outputMode_loc, outputMode);

    int locTexture = shader->GetUniformLocation("textureImage");
    glUniform1i(locTexture, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, originalImage->GetTextureID());

    locTexture = shader->GetUniformLocation("textureMark");
    glUniform1i(locTexture, 1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, watermarkImage->GetTextureID());

    locTexture = shader->GetUniformLocation("textureDetect");
    glUniform1i(locTexture, 2);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, detected);

    RenderMesh(meshes["quad"], shader, glm::mat4(1));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Tema2::ShowImage(GLuint image, glm::ivec2 size)
{
    window->SetSize(size.x, size.y);

    glViewport(0, 0, size.x, size.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto shader = shaders["Simple"];
    shader->Use();

    int flip_loc = shader->GetUniformLocation("flipVertical");
    glUniform1i(flip_loc, 0);

    int screenSize_loc = shader->GetUniformLocation("imageSize");
    glUniform2i(screenSize_loc, size.x, size.y);

    int outputMode_loc = shader->GetUniformLocation("outputMode");
    glUniform1i(outputMode_loc, outputMode);

    int locTexture = shader->GetUniformLocation("textureImage");
    glUniform1i(locTexture, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, image);

    RenderMesh(meshes["quad"], shader, glm::mat4(1));
}

void Tema2::DeleteTexture(GLuint& texture)
{
    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // Delete the texture
    glDeleteTextures(1, &texture);
}

void Tema2::Update(float deltaTimeSeconds)
{
    // Use sobel framebuffer
    
    Sobel(fbsobelImage, originalImage->GetTextureID(), glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight()));
    Sobel(fbsobelMark, watermarkImage->GetTextureID(), glm::ivec2(watermarkImage->GetWidth(), watermarkImage->GetHeight()));

    Detect();

    Remove();

    ShowImage(removed, glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight()));

    //ShowImage(watermarkImage->GetTextureID(), glm::ivec2(watermarkImage->GetWidth(), watermarkImage->GetHeight()));
    
    // SAVING
    if (saveScreenToImage)
    {
        saveScreenToImage = false;

        GLenum format = GL_RGB;
        if (originalImage->GetNrChannels() == 4)
        {
            format = GL_RGBA;
        }

        glReadPixels(0, 0, originalImage->GetWidth(), originalImage->GetHeight(), format, GL_UNSIGNED_BYTE, processedImage->GetImageData());
        processedImage->UploadNewData(processedImage->GetImageData());
        SaveImage("shader_processing_" + std::to_string(outputMode));

        window->SetSize(originalImage->GetWidth(), originalImage->GetHeight());
    }
    finished = true;
}


void Tema2::FrameEnd()
{
    //DrawCoordinateSystem();
}


void Tema2::OnFileSelected(const std::string &fileName)
{
    if (fileName.size())
    {
        std::cout << fileName << endl;
        originalImage = TextureManager::LoadTexture(fileName, nullptr, "image", true, true);
        processedImage = TextureManager::LoadTexture(fileName, nullptr, "newImage", true, true);

        //float aspectRatio = static_cast<float>(originalImage->GetWidth()) / originalImage->GetHeight();
        window->SetSize(originalImage->GetWidth(), originalImage->GetHeight());

        InitFramebuffer(glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight()), &sobelImage, &fbsobelImage);
        InitFramebuffer(glm::ivec2(watermarkImage->GetWidth(), watermarkImage->GetHeight()), &sobelMark, &fbsobelMark);
        InitFramebuffer(glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight()), &detected, &fbdetected);
        InitFramebuffer(glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight()), &removed, &fbremoved);
    }
}


void Tema2::GrayScale()
{
    unsigned int channels = originalImage->GetNrChannels();
    unsigned char* data = originalImage->GetImageData();
    unsigned char* newData = processedImage->GetImageData();

    if (channels < 3)
        return;

    glm::ivec2 imageSize = glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight());

    for (int i = 0; i < imageSize.y; i++)
    {
        for (int j = 0; j < imageSize.x; j++)
        {
            int offset = channels * (i * imageSize.x + j);

            // Reset save image data
            char value = static_cast<char>(data[offset + 0] * 0.2f + data[offset + 1] * 0.71f + data[offset + 2] * 0.07);
            memset(&newData[offset], value, 3);
        }
    }

    processedImage->UploadNewData(newData);
}

void Tema2::Blur(int radius) {
    unsigned int channels = originalImage->GetNrChannels();
    glm::ivec2 imageSize = glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight());
    unsigned char* data = originalImage->GetImageData();
    unsigned char* newData = processedImage->GetImageData();

    for (int i = 0; i < imageSize.y; ++i) {
        for (int j = 0; j < imageSize.x; ++j) {
            int offset = channels * (i * imageSize.x + j);
            float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;

            // Apply the blur within the specified radius
            for (int k = -radius; k <= radius; ++k) {
                for (int l = -radius; l <= radius; ++l) {
                    int row = glm::clamp(i + k, 0, static_cast<int>(imageSize.y) - 1);
                    int col = glm::clamp(j + l, 0, static_cast<int>(imageSize.x) - 1);
                    int localOffset = channels * (row * imageSize.x + col);

                    // Weighted sum for each channel
                    sumR += data[localOffset + 0] * 0.2f;
                    sumG += data[localOffset + 1] * 0.71f;
                    sumB += data[localOffset + 2] * 0.07;
                }
            }

            // Update the pixel value in the processed image
            newData[offset + 0] = static_cast<unsigned char>(glm::clamp(sumR, 0.0f, 255.0f));
            newData[offset + 1] = static_cast<unsigned char>(glm::clamp(sumG, 0.0f, 255.0f));
            newData[offset + 2] = static_cast<unsigned char>(glm::clamp(sumB, 0.0f, 255.0f));
        }
    }

    processedImage->UploadNewData(newData);
}


void Tema2::SaveImage(const std::string &fileName)
{
    cout << "Saving image! ";
    processedImage->SaveToFile((fileName + ".png").c_str());
    cout << "[Done]" << endl;
}


void Tema2::OpenDialog()
{
    std::vector<std::string> filters =
    {
        "Image Files", "*.png *.jpg *.jpeg *.bmp",
        "All Files", "*"
    };

    auto selection = pfd::open_file("Select a file", ".", filters).result();
    if (!selection.empty())
    {
        std::cout << "User selected file " << selection[0] << "\n";
        OnFileSelected(selection[0]);
    }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F || key == GLFW_KEY_ENTER || key == GLFW_KEY_SPACE)
    {
        OpenDialog();
    }

    if (key == GLFW_KEY_E)
    {
        gpuProcessing = !gpuProcessing;
        if (gpuProcessing == false)
        {
            outputMode = 0;
        }
        cout << "Processing on GPU: " << (gpuProcessing ? "true" : "false") << endl;
    }

    if (key - GLFW_KEY_0 >= 0 && key < GLFW_KEY_4)
    {
        outputMode = key - GLFW_KEY_0;

        if (gpuProcessing == false)
        {
            outputMode = 0;
            GrayScale();
        }
    }

    if (key == GLFW_KEY_S && mods & GLFW_MOD_CONTROL)
    {
        if (!gpuProcessing)
        {
            SaveImage("processCPU_" + std::to_string(outputMode));
        } else {
            saveScreenToImage = true;
        }
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Tema2::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
