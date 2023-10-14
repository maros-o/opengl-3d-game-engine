#include "Engine.h"

void Engine::run(OpenGLContext *context, Camera *camera, InputManager *input_manager, Renderer *renderer,
                 TransformableComposite *transformable_root) {
    while (!context->should_close()) {
        InputManager::poll_events();
        input_manager->update();

        transformable_root->update_model_matrix();

        Renderer::clear();
        renderer->render_objects(camera);

        context->swap_buffers();
    }
}
