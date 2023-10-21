#include "OpenGLContext.h"
#include "Renderer.h"
#include "buffers/EBO.h"
#include "Engine.h"
#include "Model.h"
#include "../res/vertices.h"
#include "../res/indices.h"
#include "Camera.h"
#include "Light.h"
#include "InputManager.h"
#include <assimp/Importer.hpp>
#include "../res/3d_models/plain.h"
#include "../res/3d_models/suzi_flat.h"
#include "../res/3d_models/suzi_smooth.h"
#include "../res/3d_models/sphere.h"
#include "../res/3d_models/gift.h"
#include "../res/3d_models/bushes.h"
#include "../res/3d_models/tree.h"
#include "RenderObject/RenderObjectLeaf.h"


int main() {
    OpenGLContext &context = OpenGLContext::get_instance().init(900, 600, "ZPG - MEC0045");

    InputManager &input_manager = InputManager::get_instance().init();

    auto camera = new Camera(context.get_screen_width(), context.get_screen_height());

    auto light = new Light();

    auto shader_phong = new ShaderProgram{"../res/shaders/phong.shader", camera};
    shader_phong->set_light(light);
    auto shader_yellow = new ShaderProgram{"../res/shaders/yellow.shader", camera};

    auto vbo_sphere = new VBO{sphere, 6};
    auto vao_sphere = new VAO(vbo_sphere);
    vao_sphere->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
    vao_sphere->link_attributes(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    auto sphere_model = new Model{"sphere", vao_sphere, shader_phong};

    auto sun_model = new Model{"sun", vao_sphere, shader_yellow};

    RenderObject *sphere_1 = new RenderObjectLeaf{sphere_model};
    RenderObject *sphere_2 = new RenderObjectLeaf{sphere_model};
    RenderObject *sphere_3 = new RenderObjectLeaf{sphere_model};
    RenderObject *sphere_4 = new RenderObjectLeaf{sphere_model};

    sphere_1->set_position(glm::vec3(2.0f, 0.0f, 0.0f));
    sphere_2->set_position(glm::vec3(-2.0f, 0.0f, 0.0f));
    sphere_3->set_position(glm::vec3(0.0f, 0.0f, 2.0f));
    sphere_4->set_position(glm::vec3(0.0f, 0.0f, -2.0f));


    RenderObject *sun = new RenderObjectLeaf{sun_model};
    light->set_render_object(sun);

    auto renderer = new Renderer();
    renderer->add_object(sphere_1);
    renderer->add_object(sphere_2);
    renderer->add_object(sphere_3);
    renderer->add_object(sphere_4);
    renderer->add_object(sun);


    input_manager.register_key_down_callback(GLFW_KEY_UP, [&light]() {
        light->move(glm::vec3(0.0f, 0.0f, .1f));
    });
    input_manager.register_key_down_callback(GLFW_KEY_DOWN, [&light]() {
        light->move(glm::vec3(0.0f, 0.0f, -.1f));
    });
    input_manager.register_key_down_callback(GLFW_KEY_LEFT, [&light]() {
        light->move(glm::vec3(-.1f, 0.0f, 0.0f));
    });
    input_manager.register_key_down_callback(GLFW_KEY_RIGHT, [&light]() {
        light->move(glm::vec3(.1f, 0.0f, 0.0f));
    });
    input_manager.register_key_down_callback(GLFW_KEY_PAGE_UP, [&light]() {
        light->move(glm::vec3(0.0f, .1f, 0.0f));
    });
    input_manager.register_key_down_callback(GLFW_KEY_PAGE_DOWN, [&light]() {
        light->move(glm::vec3(0.0f, -.1f, 0.0f));
    });


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
                InputManager::set_cursor_position(context.get_screen_width() / 2, context.get_screen_height() / 2);
            });
    input_manager.register_window_resize_callback([&camera](unsigned short width, unsigned short height) {
        printf("window resized: (%d, %d)\n", width, height);
        camera->window_resize(width, height);
        OpenGLContext::set_viewport(width, height);
    });
    input_manager.register_key_press_callback(GLFW_KEY_ESCAPE, [&context]() {
        context.close();
    });


    Engine::run(renderer);
}
