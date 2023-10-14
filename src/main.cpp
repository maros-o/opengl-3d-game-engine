#include "OpenGLContext.h"
#include "Renderer.h"
#include "EBO.h"
#include "Engine.h"
#include "Model.h"
#include "../res/vertices.h"
#include "../res/indices.h"
#include "Camera.h"
#include "InputManager.h"
#include "TransformableComposite.h"
#include <assimp/Importer.hpp>

int main() {
    auto context = new OpenGLContext(900, 600, "ZPG - MEC0045");

    auto camera = new Camera(context->get_width(), context->get_height());

    auto shader_program_texture = new ShaderProgram{"../res/shaders/texture.shader", camera};
    auto shader_program_rgb = new ShaderProgram{"../res/shaders/rgb.shader", camera};


    auto vao_triangle = new VAO{new VBO{vertices_triangle_rgb},
                                new EBO{indices_triangle}};
    vao_triangle->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
    vao_triangle->link_attributes(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    auto *triangle_model = new Model{"rgb_triangle", vao_triangle, shader_program_rgb};


    auto vao_fei_cube = new VAO{new VBO{vertices_cube_texture},
                                new EBO{indices_cube}};
    vao_fei_cube->link_attributes(0, 3, GL_FLOAT, 5 * sizeof(GLfloat), nullptr);
    vao_fei_cube->link_attributes(1, 2, GL_FLOAT, 5 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    auto *fei_cube_model = new Model{"fei_cube", vao_fei_cube, shader_program_texture,
                                     new Texture{"../res/textures/fei.png"}};


    auto triangle_1 = new RenderObject{triangle_model};
    auto triangle_2 = new RenderObject{triangle_model};
    auto fei_cube_1 = new RenderObject{fei_cube_model};
    auto fei_cube_2 = new RenderObject{fei_cube_model};

    auto transformable_root = new TransformableComposite({triangle_1, triangle_2, fei_cube_1});

    auto renderer = new Renderer();
    renderer->add_object(fei_cube_1);
    renderer->add_object(triangle_1);
    renderer->add_object(triangle_2);
    renderer->add_object(fei_cube_2);

    triangle_1->translate(glm::vec3(1.f, 0.0f, 0.0f));
    triangle_1->update_model_matrix();

    triangle_2->translate(glm::vec3(-1.f, 0.0f, 0.0f));
    triangle_2->update_model_matrix();

    fei_cube_1->translate(glm::vec3(0.0f, 0.0f, -3.0f));
    fei_cube_1->update_model_matrix();

    InputManager &input_manager = InputManager::get_instance();
    input_manager.init();

    input_manager.register_key_down_callback(GLFW_KEY_W, [&camera]() {
        camera->move(CameraMovement::FORWARD);
        printf("key down: W\n");
    });
    input_manager.register_key_down_callback(GLFW_KEY_S, [&camera]() {
        camera->move(CameraMovement::BACKWARD);
        printf("key down: S\n");
    });
    input_manager.register_key_down_callback(GLFW_KEY_A, [&camera]() {
        camera->move(CameraMovement::LEFT);
        printf("key down: A\n");
    });
    input_manager.register_key_down_callback(GLFW_KEY_D, [&camera]() {
        camera->move(CameraMovement::RIGHT);
        printf("key down: D\n");
    });
    input_manager.register_key_down_callback(GLFW_KEY_SPACE, [&camera]() {
        camera->move(CameraMovement::UP);
        printf("key down: SPACE\n");
    });
    input_manager.register_key_down_callback(GLFW_KEY_LEFT_CONTROL, [&camera]() {
        camera->move(CameraMovement::DOWN);
        printf("key down: LEFT_CONTROL\n");
    });

    input_manager.register_cursor_position_callback(
            [&camera, &context](unsigned short pos_x, unsigned short pos_y) {
                printf("mouse moved: (%d, %d)\n", pos_x, pos_y);
                camera->rotate(pos_x, pos_y);
                InputManager::set_cursor_position(context->get_width() / 2, context->get_height() / 2);
            });

    input_manager.register_window_resize_callback([&camera](unsigned short width, unsigned short height) {
        printf("window resized: (%d, %d)\n", width, height);
        camera->window_resize(width, height);
        OpenGLContext::set_viewport(width, height);
    });

    input_manager.register_key_press_callback(GLFW_KEY_ESCAPE, [&context]() {
        context->close();
    });


    Engine::run(context, renderer, transformable_root);
}
