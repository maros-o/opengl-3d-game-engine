#pragma once

#include "Renderer.h"
#include "InputManager.h"
#include "TransformableComposite.h"

class Engine {
public:
    static void run(OpenGLContext *context, Camera *camera, InputManager *input_manager, Renderer *renderer,
                    TransformableComposite *transformable_root);
};