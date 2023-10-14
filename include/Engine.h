#pragma once

#include "Renderer.h"
#include "InputManager.h"
#include "TransformableComposite.h"

class Engine {
public:
    static void run(OpenGLContext *context, Renderer *renderer,
                    TransformableComposite *transformable_root);
};