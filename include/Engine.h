#pragma once

#include "Renderer.h"
#include "OrthographicCamera.h"

class Engine {
public:
    static void run(OpenGLContext *context, OrthographicCamera *camera, Renderer *renderer);
};