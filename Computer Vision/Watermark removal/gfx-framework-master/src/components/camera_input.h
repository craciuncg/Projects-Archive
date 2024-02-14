#pragma once

#include "components/camera.h"
#include "core/window/input_controller.h"

#define CAM_FREECAM // comment this line when camera movement is required to not be freecam

namespace gfxc
{
    class CameraInput : public InputController
    {
    public:
        explicit CameraInput(Camera *camera);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;

    public:
        Camera *camera;
    };
}
