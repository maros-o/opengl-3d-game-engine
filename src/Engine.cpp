#include "Engine.h"

void Engine::run(OpenGLContext *context, Renderer *renderer) {
    InputManager &input_manager = InputManager::get_instance();

    while (!context->should_close()) {
        InputManager::poll_events();
        input_manager.update();

        Renderer::clear();
        renderer->render_objects();

        context->swap_buffers();
    }
}
