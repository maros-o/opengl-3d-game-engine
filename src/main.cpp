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
#include "Scene.h"


int main() {
    OpenGLContext &context = OpenGLContext::get_instance().init(900, 600, "ZPG - MEC0045");

    InputManager &input_manager = InputManager::get_instance().init();

    auto camera = new Camera(context.get_screen_width(), context.get_screen_height());

    auto light = new Light();

    auto shader_yellow = new ShaderProgram{"../shaders/yellow.glsl", camera};
    auto shader_blue = new ShaderProgram{"../shaders/blue.glsl", camera};
    auto shader_lambert = new ShaderProgram{"../shaders/lambert.glsl", camera, light};
    auto shader_phong = new ShaderProgram{"../shaders/phong.glsl", camera, light};
    auto shader_blinn = new ShaderProgram{"../shaders/blinn.glsl", camera, light};

    auto vbo_sphere = new VBO{sphere, 6};
    auto vao_sphere = new VAO(vbo_sphere);
    vao_sphere->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
    vao_sphere->link_attributes(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));

    auto vbo_suzi = new VBO{suzi_smooth, 6};
    auto vao_suzi = new VAO(vbo_suzi);
    vao_suzi->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
    vao_suzi->link_attributes(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));

    auto blue_model = new Model{"blue_sphere", vao_sphere, shader_blue};
    auto yellow_model = new Model{"yellow_sphere", vao_sphere, shader_yellow};
    auto lambert_model = new Model{"lambert_sphere", vao_sphere, shader_lambert};
    auto phong_model = new Model{"phong_sphere", vao_sphere, shader_phong};
    auto blinn_model = new Model{"blinn_sphere", vao_sphere, shader_blinn};

    auto blue_suzi_model = new Model{"blue_suzi", vao_suzi, shader_blue};
    auto lambert_suzi_model = new Model{"lambert_suzi", vao_suzi, shader_lambert};


    RenderObject *a_sphere_1 = new RenderObjectLeaf{blinn_model};

    RenderObject *b_sphere_1 = new RenderObjectLeaf{phong_model};
    RenderObject *b_sphere_2 = new RenderObjectLeaf{blue_model};
    RenderObject *b_sphere_3 = new RenderObjectLeaf{lambert_model};
    RenderObject *b_sphere_4 = new RenderObjectLeaf{blinn_model};

    RenderObject *c_sphere_1 = new RenderObjectLeaf{phong_model};
    RenderObject *c_suzi_1 = new RenderObjectLeaf{blue_suzi_model};
    RenderObject *c_suzi_2 = new RenderObjectLeaf{lambert_suzi_model};
    RenderObject *c_sphere_2 = new RenderObjectLeaf{blinn_model};


    light->set_position(glm::vec3(0.0f, 5.0f, 0.0f));
    camera->set_position(glm::vec3(0.0f, -5.0f, 0.0f));
    camera->set_pitch_yaw(90.0f, 0.0f);

    a_sphere_1->set_position(glm::vec3(0.0f, 0.0f, 0.0f));

    b_sphere_1->set_position(glm::vec3(2.0f, 0.0f, 0.0f));
    b_sphere_2->set_position(glm::vec3(-2.0f, 0.0f, 0.0f));
    b_sphere_3->set_position(glm::vec3(0.0f, 0.0f, 2.0f));
    b_sphere_4->set_position(glm::vec3(0.0f, 0.0f, -2.0f));

    c_sphere_1->set_position(glm::vec3(2.0f, 0.0f, 0.0f));
    c_sphere_2->set_position(glm::vec3(0.0f, 0.0f, -2.0f));
    c_suzi_1->set_position(glm::vec3(-2.0f, 0.0f, 0.0f));
    c_suzi_2->set_position(glm::vec3(0.0f, 0.0f, 2.0f));


    RenderObject *sun = new RenderObjectLeaf{yellow_model};
    sun->scale(0.2f);
    light->set_render_object(sun);

    auto scene_a = new Scene{"scene_a", {sun, a_sphere_1}};
    auto scene_b = new Scene{"scene_b", {sun, b_sphere_1, b_sphere_2, b_sphere_3, b_sphere_4}};
    auto scene_c = new Scene{"scene_c", {sun, c_sphere_1, c_sphere_2, c_suzi_1, c_suzi_2}};

    // inputs
    {
        float amount = 0.01f;
        input_manager.register_key_down_callback(GLFW_KEY_1, [&light, amount]() {
            light->set_ambient_strength(light->get_ambient_strength() + amount);
        });
        input_manager.register_key_down_callback(GLFW_KEY_2, [&light, amount]() {
            light->set_ambient_strength(light->get_ambient_strength() - amount);
        });
        input_manager.register_key_down_callback(GLFW_KEY_3, [&light, amount]() {
            light->set_diffuse_strength(light->get_diffuse_strength() + amount);
        });
        input_manager.register_key_down_callback(GLFW_KEY_4, [&light, amount]() {
            light->set_diffuse_strength(light->get_diffuse_strength() - amount);
        });
        input_manager.register_key_down_callback(GLFW_KEY_5, [&light, amount]() {
            light->set_specular_strength(light->get_specular_strength() + amount);
        });
        input_manager.register_key_down_callback(GLFW_KEY_6, [&light, amount]() {
            light->set_specular_strength(light->get_specular_strength() - amount);
        });
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
        });
        input_manager.register_key_down_callback(GLFW_KEY_S, [&camera]() {
            camera->move(CameraMovement::BACKWARD);
        });
        input_manager.register_key_down_callback(GLFW_KEY_A, [&camera]() {
            camera->move(CameraMovement::LEFT);
        });
        input_manager.register_key_down_callback(GLFW_KEY_D, [&camera]() {
            camera->move(CameraMovement::RIGHT);
        });
        input_manager.register_key_down_callback(GLFW_KEY_SPACE, [&camera]() {
            camera->move(CameraMovement::UP);
        });
        input_manager.register_key_down_callback(GLFW_KEY_LEFT_CONTROL, [&camera]() {
            camera->move(CameraMovement::DOWN);
        });
        input_manager.register_cursor_position_callback(
                [&camera, &context](unsigned short pos_x, unsigned short pos_y) {
                    camera->rotate(pos_x, pos_y);
                    InputManager::set_cursor_position(context.get_screen_width() / 2, context.get_screen_height() / 2);
                });
        input_manager.register_window_resize_callback([&camera](unsigned short width, unsigned short height) {
            camera->window_resize(width, height);
            OpenGLContext::set_viewport(width, height);
        });
        input_manager.register_key_press_callback(GLFW_KEY_ESCAPE, [&context]() {
            context.close();
        });
    }

    //scene_a->play();
    scene_b->play();
    //scene_c->play();
}
