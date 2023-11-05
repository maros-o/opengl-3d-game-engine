#include <random>
#include <assimp/Importer.hpp>

#include "OpenGLContext.h"
#include "Renderer.h"
#include "Model.h"
#include "Camera.h"
#include "Light/PointLight.h"
#include "Light/DirectionalLight.h"
#include "InputManager.h"
#include "Scene.h"
#include "../include/Transform/TransformComposite.h"

#include "../res/vertices.h"
#include "../res/indices.h"
#include "../res/3d_models/plain.h"
#include "../res/3d_models/suzi_flat.h"
#include "../res/3d_models/suzi_smooth.h"
#include "../res/3d_models/sphere.h"
#include "../res/3d_models/gift.h"
#include "../res/3d_models/bushes.h"
#include "../res/3d_models/tree.h"
#include "Light/SpotLight.h"

static float random_normalized_float() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    return dis(gen);
}

int main() {
    OpenGLContext &context = OpenGLContext::get_instance().init(900, 600, "ZPG - MEC0045");

    InputManager &input_manager = InputManager::get_instance().init();

    auto camera = new Camera(context.get_screen_width(), context.get_screen_height());

    auto light_1 = new PointLight();
    auto light_2 = new PointLight();
    auto dir_light_1 = new DirectionalLight();
    auto spot_light_1 = new SpotLight();
    spot_light_1->set_cut_off(0.95f);

    // shaders
    auto shader_constant = new ShaderProgram{"../shaders/constant.glsl", camera};
    auto shader_blinn = new ShaderProgram{"../shaders/blinn.glsl", camera,
                                          {spot_light_1}};

    // vaos
    auto vao_sphere = new VAO(VBO{sphere, 6});
    auto vao_suzi = new VAO(VBO{suzi_smooth, 6});
    auto vao_plain = new VAO(VBO{plain, 6});
    auto vao_bushes = new VAO(VBO{bushes, 6});
    auto vao_tree = new VAO(VBO{tree, 6});
    auto vao_gift = new VAO(VBO{gift, 6});
    {
        vao_sphere->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
        vao_sphere->link_attributes(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));

        vao_suzi->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
        vao_suzi->link_attributes(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));

        vao_plain->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
        vao_plain->link_attributes(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));

        vao_bushes->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
        vao_bushes->link_attributes(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));

        vao_tree->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
        vao_tree->link_attributes(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));

        vao_gift->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
        vao_gift->link_attributes(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    }

    // materials
    auto material_yellow = new Material{glm::vec3(255.f, 255.f, 0.f) / 255.f};
    auto material_blue = new Material{glm::vec3(98.f, 98.f, 255.f) / 255.f};
    auto material_green = new Material{glm::vec3(25.f, 117.f, 25.f) / 255.f};
    auto material_grey = new Material{glm::vec3(100.f, 100.f, 100.f) / 255.f};
    auto material_red = new Material{glm::vec3(255.f, 25.f, 25.f) / 255.f};

    // models
    auto model_sphere = new Model{"model_sphere", vao_sphere};
    auto model_tree = new Model{"model_tree", vao_tree};
    auto model_plain = new Model{"model_plain", vao_plain};
    auto model_bushes = new Model{"model_bushes", vao_bushes};
    auto model_gift = new Model{"model_gift", vao_gift};

    auto *light_1_render_obj = new RenderObject{model_gift, shader_constant, material_yellow};
    spot_light_1->get_transform()->attach(light_1_render_obj->get_transform());

    spot_light_1->set_camera(camera);

    auto *light_2_render_obj = new RenderObject{model_sphere, shader_constant, material_yellow};
    light_2->get_transform()->attach(light_2_render_obj->get_transform());


    auto render_objects = std::vector<RenderObject *>{light_2_render_obj};

    auto floor = new RenderObject{model_plain, shader_blinn, material_green};
    render_objects.push_back(floor);
    floor->get_transform()->scale(1000.f);
    floor->get_material()->set_specular_strength(0.f);

    float max_x = 30.f;
    float max_z = 30.f;

    // trees
    for (int x = 0; x < 10; x++) {
        for (int z = 0; z < 10; z++) {
            auto obj = new RenderObject{model_tree, shader_blinn, material_blue};
            obj->get_transform()->set_position(glm::vec3(max_x * (random_normalized_float() - 1.f),
                                                         random_normalized_float() - 1.f,
                                                         max_z * (random_normalized_float() - 1.f)));
            obj->get_transform()->scale(random_normalized_float());
            obj->get_transform()->set_rotation(
                    glm::vec3((random_normalized_float() - 0.5f) * 90.f, 0.f,
                              (random_normalized_float() - 0.5f) * 90.f));
            render_objects.push_back(obj);
        }
    }
    // bushes
    for (int x = 0; x < 10; x++) {
        for (int z = 0; z < 10; z++) {
            auto obj = new RenderObject{model_bushes, shader_blinn, material_green};
            obj->get_transform()->set_position(glm::vec3(max_x * (random_normalized_float() - 1.f),
                                                         random_normalized_float() - 1.f,
                                                         max_z * (random_normalized_float() - 1.f)));
            obj->get_transform()->scale(random_normalized_float() * 4);
            render_objects.push_back(obj);
        }
    }
    // gifts
    for (int x = 0; x < 10; x++) {
        for (int z = 0; z < 10; z++) {
            auto obj = new RenderObject{model_gift, shader_blinn, material_red};
            obj->get_transform()->scale(random_normalized_float() * 4);
            obj->get_transform()->set_position(glm::vec3(max_x * (random_normalized_float() - 1.f),
                                                         random_normalized_float() - 1.f,
                                                         max_z * (random_normalized_float() - 1.f)));
            render_objects.push_back(obj);
        }
    }

    auto scene = new Scene{"scene_5", render_objects,
                           {camera->get_transform_mount()}};
    scene->set_on_create([&camera, &spot_light_1, &light_2, &light_1]() {
        camera->set_position(glm::vec3(0.f, 5.f, 0.f));
        spot_light_1->set_position(glm::vec3(0.f, 0.f, -1.f));
        light_2->set_position(glm::vec3(0.f, 10.f, 0.f));
    });
    scene->set_on_update([&camera, &light_1, &light_2]() {

    });


    // inputs
    {
        input_manager.register_key_down_callback(GLFW_KEY_UP, [&light_1]() {
            light_1->move(glm::vec3(0.0f, 0.0f, .1f));
        });
        input_manager.register_key_down_callback(GLFW_KEY_DOWN, [&light_1]() {
            light_1->move(glm::vec3(0.0f, 0.0f, -.1f));
        });
        input_manager.register_key_down_callback(GLFW_KEY_LEFT, [&light_1]() {
            light_1->move(glm::vec3(-.1f, 0.0f, 0.0f));
        });
        input_manager.register_key_down_callback(GLFW_KEY_RIGHT, [&light_1]() {
            light_1->move(glm::vec3(.1f, 0.0f, 0.0f));
        });
        input_manager.register_key_down_callback(GLFW_KEY_PAGE_UP, [&light_1]() {
            light_1->move(glm::vec3(0.0f, .1f, 0.0f));
        });
        input_manager.register_key_down_callback(GLFW_KEY_PAGE_DOWN, [&light_1]() {
            light_1->move(glm::vec3(0.0f, -.1f, 0.0f));
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

    scene->play();
}
