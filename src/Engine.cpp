#include "Engine.h"

void Engine::run(OpenGLContext *context, OrthographicCamera *camera, Renderer *renderer) {
    while (!context->should_close()) {
        OpenGLContext::poll_events();

        Renderer::clear();
        renderer->render_all_objects(camera);

        context->swap_buffers();
    }
}
