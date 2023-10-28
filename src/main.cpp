#include "OpenGLContext.h"
#include "Renderer.h"
#include "buffers/EBO.h"
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

    auto shader_constant = new ShaderProgram{"../shaders/constant.glsl", camera};
    auto shader_blinn = new ShaderProgram{"../shaders/blinn.glsl", camera, light};

    auto vbo_sphere = new VBO{sphere, 6};
    auto vao_sphere = new VAO(vbo_sphere);
    vao_sphere->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
    vao_sphere->link_attributes(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));

    auto vbo_suzi = new VBO{suzi_smooth, 6};
    auto vao_suzi = new VAO(vbo_suzi);
    vao_suzi->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
    vao_suzi->link_attributes(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));

    auto vbo_plain = new VBO{plain, 6};
    auto vao_plain = new VAO(vbo_plain);
    vao_plain->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
    vao_plain->link_attributes(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));

    auto vbo_bushes = new VBO{bushes, 6};
    auto vao_bushes = new VAO(vbo_bushes);
    vao_bushes->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
    vao_bushes->link_attributes(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));

    auto vbo_tree = new VBO{tree, 6};
    auto vao_tree = new VAO(vbo_tree);
    vao_tree->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
    vao_tree->link_attributes(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));

    auto vbo_gift = new VBO{gift, 6};
    auto vao_gift = new VAO(vbo_gift);
    vao_gift->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
    vao_gift->link_attributes(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));

    auto material_yellow = new Material{glm::vec3(255.f, 255.f, 0.f) / 255.f};
    auto material_blue = new Material{glm::vec3(98.f, 98.f, 255.f) / 255.f};
    auto material_green = new Material{glm::vec3(25.f, 117.f, 25.f) / 255.f};

    auto model_sun = new Model{"model_sun", vao_sphere, shader_constant, material_yellow};
    auto model_sphere = new Model{"model_sphere", vao_sphere, shader_blinn, material_blue};
    auto model_tree = new Model{"model_tree", vao_tree, shader_blinn, material_blue};
    auto model_plain = new Model{"model_plain", vao_plain, shader_blinn, material_green};

    /*
    auto model_bushes = new Model{"model_bushes", vao_bushes, shader_blinn, glm::vec3{255.f, 25.f, 255.f} / 255.f};
    auto model_gift = new Model{"model_gift", vao_gift, shader_blinn, glm::vec3{255.f, 25.f, 255.f} / 255.f};
    auto model_plain = new Model{"model_plain", vao_plain, shader_blinn, glm::vec3{25.f, 117.f, 25.f} / 255.f};
    auto model_suzi = new Model{"model_suzi", vao_suzi, shader_blinn, glm::vec3{255.f, 25.f, 255.f} / 255.f};
    auto model_sphere = new Model{"model_sphere", vao_sphere, shader_blinn, glm::vec3(98.f, 98.f, 255.f) / 255.f};
    */


    RenderObject *sun = new RenderObjectLeaf{model_sun};
    sun->scale(0.2f);
    light->set_render_object(sun);

    // scene 1
    auto sphere1 = new RenderObjectLeaf{model_sphere};
    sphere1->set_position(glm::vec3(2.f, 0.f, 0.f));
    auto sphere2 = new RenderObjectLeaf{model_sphere};
    sphere2->set_position(glm::vec3(-2.f, 0.f, 0.f));
    auto sphere3 = new RenderObjectLeaf{model_sphere};
    sphere3->set_position(glm::vec3(0.f, 2.f, 0.f));
    auto sphere4 = new RenderObjectLeaf{model_sphere};
    sphere4->set_position(glm::vec3(0.f, -2.f, 0.f));

    auto scene_1 = new Scene{"scene_1", {sun, sphere1, sphere2, sphere3, sphere4}};
    scene_1->set_on_create([&camera, &light]() {
        camera->set_position(glm::vec3(0.f, 0.f, 6.f));
        camera->set_pitch_yaw(0.f, 0.f);
        light->set_position(glm::vec3(0.f, 0.f, 0.f));
    });

    // scene 2


    // scene 5
    std::vector<RenderObject *> scene5_render_objects{sun};

    auto floor = new RenderObjectLeaf{model_plain};
    scene5_render_objects.push_back(floor);
    floor->scale(100.f);

    for (int x = 0; x < 10; x++) {
        for (int z = 0; z < 10; z++) {
            auto tree_obj = new RenderObjectLeaf{model_tree};
            tree_obj->set_position(glm::vec3(x * 10.f, 0.f, z * 10.f));
            scene5_render_objects.push_back(tree_obj);
        }
    }

    auto scene_5 = new Scene{"scene_forest", {scene5_render_objects}};

    // inputs
    {
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
                [&camera, &context, &input_manager](unsigned short pos_x, unsigned short pos_y) {
                    if (input_manager.is_key_down(GLFW_KEY_K)) {
                        camera->rotate(pos_x, pos_y);
                        InputManager::set_cursor_position(context.get_screen_width() / 2,
                                                          context.get_screen_height() / 2);
                        glfwSetInputMode(context.get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    } else {
                        glfwSetInputMode(context.get_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                    }
                });
        input_manager.register_window_resize_callback([&camera](unsigned short width, unsigned short height) {
            camera->window_resize(width, height);
            OpenGLContext::set_viewport(width, height);
        });
        input_manager.register_key_press_callback(GLFW_KEY_ESCAPE, [&context]() {
            context.close();
        });
    }

    scene_1->play();
}
