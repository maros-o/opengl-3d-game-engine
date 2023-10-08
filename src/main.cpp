#include "OpenGLContext.h"
#include "Renderer.h"
#include "EBO.h"
#include "Engine.h"
#include "Model.h"
#include "../res/vertices.h"
#include "../res/indices.h"
#include "OrthoCamera.h"
#include "InputManager.h"

int main() {
    auto context = new OpenGLContext(900, 600, "ZPG - MEC0045");

    auto shader_program_rgb = new ShaderProgram{"../res/shaders/rgb.shader"};
    auto shader_program_texture = new ShaderProgram{"../res/shaders/texture.shader"};


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

    auto renderer = new Renderer();
    renderer->add_object(fei_cube_1);
    renderer->add_object(triangle_1);
    renderer->add_object(triangle_2);

    triangle_1->translate(glm::vec3(1.f, 0.0f, 0.0f));
    triangle_1->update_model_matrix();

    triangle_2->translate(glm::vec3(-1.f, 0.0f, 0.0f));
    triangle_2->update_model_matrix();

    triangle_1->attach_child(fei_cube_1);
    fei_cube_1->attach_child(triangle_2);

    auto camera = new OrthoCamera(context->get_width(), context->get_height());

    float speed = 0.05f;
    auto input_manager = new InputManager(context->get_window());
    input_manager->register_key_callback(GLFW_KEY_ESCAPE, [&context]() {
        context->close();
    });
    input_manager->register_key_callback(GLFW_KEY_A, [&triangle_1, speed]() {
        triangle_1->translate(glm::vec3(-speed, 0.0f, 0.0f));
        triangle_1->update_model_matrix();
    });
    input_manager->register_key_callback(GLFW_KEY_D, [&triangle_1, speed]() {
        triangle_1->translate(glm::vec3(speed, 0.0f, 0.0f));
        triangle_1->update_model_matrix();
    });
    input_manager->register_key_callback(GLFW_KEY_W, [&triangle_1, speed]() {
        triangle_1->translate(glm::vec3(0.0f, speed, 0.0f));
        triangle_1->update_model_matrix();
    });
    input_manager->register_key_callback(GLFW_KEY_S, [&triangle_1, speed]() {
        triangle_1->translate(glm::vec3(0.0f, -speed, 0.0f));
        triangle_1->update_model_matrix();
    });
    input_manager->register_key_callback(GLFW_KEY_I, [&triangle_1, speed]() {
        triangle_1->translate(glm::vec3(0.0f, 0.0f, speed));
        triangle_1->update_model_matrix();
    });
    input_manager->register_key_callback(GLFW_KEY_O, [&triangle_1, speed]() {
        triangle_1->translate(glm::vec3(0.0f, 0.0f, -speed));
        triangle_1->update_model_matrix();
    });
    input_manager->register_key_callback(GLFW_KEY_Q, [&triangle_1, speed]() {
        triangle_1->rotate_rad(speed, glm::vec3(0.0f, 0.0f, 1.0f));
        triangle_1->update_model_matrix();
    });
    input_manager->register_key_callback(GLFW_KEY_E, [&triangle_1, speed]() {
        triangle_1->rotate_rad(-speed, glm::vec3(0.0f, 0.0f, 1.0f));
        triangle_1->update_model_matrix();
    });
    input_manager->register_key_callback(GLFW_KEY_X, [&triangle_1, speed]() {
        triangle_1->scale(glm::vec3(1.0f - speed, 1.0f - speed, 1.0f - speed));
        triangle_1->update_model_matrix();
    });
    input_manager->register_key_callback(GLFW_KEY_C, [&triangle_1, speed]() {
        triangle_1->scale(glm::vec3(1.0f + speed, 1.0f + speed, 1.0f + speed));
        triangle_1->update_model_matrix();
    });

    input_manager->register_key_callback(GLFW_KEY_UP, [&fei_cube_1, speed]() {
        fei_cube_1->rotate_rad(speed, glm::vec3(1.0f, 0.0f, 0.0f));
        fei_cube_1->update_model_matrix();
    });
    input_manager->register_key_callback(GLFW_KEY_DOWN, [&fei_cube_1, speed]() {
        fei_cube_1->rotate_rad(-speed, glm::vec3(1.0f, 0.0f, 0.0f));
        fei_cube_1->update_model_matrix();
    });
    input_manager->register_key_callback(GLFW_KEY_LEFT, [&fei_cube_1, speed]() {
        fei_cube_1->rotate_rad(speed, glm::vec3(0.0f, 1.0f, 0.0f));
        fei_cube_1->update_model_matrix();
    });
    input_manager->register_key_callback(GLFW_KEY_RIGHT, [&fei_cube_1, speed]() {
        fei_cube_1->rotate_rad(-speed, glm::vec3(0.0f, 1.0f, 0.0f));
        fei_cube_1->update_model_matrix();
    });


    Engine::run(context, camera, input_manager, renderer);
}
