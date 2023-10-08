#pragma once

#include "Renderer.h"
#include "OrthoCamera.h"
#include "InputManager.h"

class Engine {
public:
    static void run(OpenGLContext *context, OrthoCamera *camera, InputManager *input_manager, Renderer *renderer);
};