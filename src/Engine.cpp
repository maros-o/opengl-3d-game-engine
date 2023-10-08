#include "Engine.h"

void Engine::run(OpenGLContext *context, OrthographicCamera *camera, Renderer *renderer) {
    float rotation = 0.0f;
    float rotation_speed = 0.001f;
    while (!context->should_close()) {
        OpenGLContext::poll_events();

        Renderer::clear();
        renderer->render_all_objects(camera, rotation);

        context->swap_buffers();

        rotation += rotation_speed;

        if (rotation > 1.f) {
            rotation_speed = -rotation_speed;
        }
        if (rotation < 0.0f) {
            rotation_speed = -rotation_speed;
        }

        printf("rotation = %f\n", rotation);
    }
}
