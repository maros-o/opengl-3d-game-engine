#include "Engine.h"

void Engine::run(OpenGLContext *context, Renderer *renderer,
                 TransformableComposite *transformable_root) {
    InputManager &input_manager = InputManager::get_instance();

    while (!context->should_close()) {
        InputManager::poll_events();
        input_manager.update();

        transformable_root->update_model_matrix();

        Renderer::clear();
        renderer->render_objects();

        context->swap_buffers();
    }
}
