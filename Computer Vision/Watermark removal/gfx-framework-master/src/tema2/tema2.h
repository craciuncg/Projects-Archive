#pragma once

#include <string>

#include "components/simple_scene.h"
#include "core/gpu/frame_buffer.h"


namespace tema
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void OpenDialog();
        void OnFileSelected(const std::string &fileName);

        // Processing effects
        void GrayScale();
        void SaveImage(const std::string &fileName);
        void Blur(int radius);

        void DeleteTexture(GLuint& texture);

        void InitFramebuffer(glm::ivec2 size, GLuint* resultTexture, GLuint* framebuffer);

        void Sobel(GLuint framebuffer, GLuint image, glm::ivec2 size);
        void Detect();
        void Remove();

        void ShowImage(GLuint image, glm::ivec2 size);

     private:
        Texture2D *originalImage;
        Texture2D *processedImage;

        Texture2D* watermarkImage;

        int outputMode;
        bool gpuProcessing;
        bool saveScreenToImage;

        bool finished = false;

        GLuint sobelImage;
        GLuint sobelMark;
        GLuint detected;
        GLuint removed;

        GLuint fbsobelImage;
        GLuint fbsobelMark;
        GLuint fbdetected;
        GLuint fbremoved;
    };
}   // namespace m2
