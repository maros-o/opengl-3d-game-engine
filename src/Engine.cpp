#include "Engine.h"

void Engine::run(OpenGLContext *context, OrthoCamera *camera, InputManager *input_manager, Renderer *renderer) {
    while (!context->should_close()) {
        InputManager::poll_events();
        input_manager->update();

        Renderer::clear();
        renderer->render_all_objects(camera);

        context->swap_buffers();
    }
}
