#include "OpenGLContext.h"
#include "Renderer.h"
#include "buffers/EBO.h"
#include "Model.h"
#include "../res/vertices.h"
#include "../res/indices.h"
#include "Camera.h"
#include "Light.h"
#include "InputManager.h"
#include "../include/Transform/TransformComposite.h"
#include <assimp/Importer.hpp>
#include "../res/3d_models/plain.h"
#include "../res/3d_models/suzi_flat.h"
#include "../res/3d_models/suzi_smooth.h"
#include "../res/3d_models/sphere.h"
#include "../res/3d_models/gift.h"
#include "../res/3d_models/bushes.h"
#include "../res/3d_models/tree.h"
#include "Scene.h"

static float random_normalized_float() {
    std::cout << static_cast<float>(rand()) / static_cast<float>(RAND_MAX) << std::endl;
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}


int main() {
    OpenGLContext &context = OpenGLContext::get_instance().init(900, 600, "ZPG - MEC0045");

    InputManager &input_manager = InputManager::get_instance().init();

    auto camera = new Camera(context.get_screen_width(), context.get_screen_height());

    auto light = new Light();

    auto shader_constant = new ShaderProgram{"../shaders/constant.glsl", camera};
    auto shader_blinn = new ShaderProgram{"../shaders/blinn.glsl", camera, light};
    auto shader_phong = new ShaderProgram{"../shaders/phong.glsl", camera, light};

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
    auto material_grey = new Material{glm::vec3(100.f, 100.f, 100.f) / 255.f};
    auto material_red = new Material{glm::vec3(255.f, 25.f, 25.f) / 255.f};

    auto model_sphere = new Model{"model_sphere", vao_sphere};
    auto model_tree = new Model{"model_tree", vao_tree};
    auto model_plain = new Model{"model_plain", vao_plain};
    auto model_bushes = new Model{"model_bushes", vao_bushes};
    auto model_gift = new Model{"model_gift", vao_gift};

    /*
    auto model_bushes = new Model{"model_bushes", vao_bushes, shader_blinn, glm::vec3{255.f, 25.f, 255.f} / 255.f};
    auto model_gift = new Model{"model_gift", vao_gift, shader_blinn, glm::vec3{255.f, 25.f, 255.f} / 255.f};
    auto model_plain = new Model{"model_plain", vao_plain, shader_blinn, glm::vec3{25.f, 117.f, 25.f} / 255.f};
    auto model_suzi = new Model{"model_suzi", vao_suzi, shader_blinn, glm::vec3{255.f, 25.f, 255.f} / 255.f};
    auto model_sphere = new Model{"model_sphere", vao_sphere, shader_blinn, glm::vec3(98.f, 98.f, 255.f) / 255.f};
    */

    auto *light_render_obj = new RenderObject{model_sphere, shader_constant, material_yellow};
    light->set_render_object(light_render_obj);

    // scene 1
    auto sphere1 = new RenderObject{model_sphere, shader_blinn, material_blue};
    sphere1->get_transform()->set_position(glm::vec3(2.f, 0.f, 0.f));
    auto sphere2 = new RenderObject{model_sphere, shader_blinn, material_blue};
    sphere2->get_transform()->set_position(glm::vec3(-2.f, 0.f, 0.f));
    auto sphere3 = new RenderObject{model_sphere, shader_blinn, material_blue};
    sphere3->get_transform()->set_position(glm::vec3(0.f, 2.f, 0.f));
    auto sphere4 = new RenderObject{model_sphere, shader_blinn, material_blue};
    sphere4->get_transform()->set_position(glm::vec3(0.f, -2.f, 0.f));

    auto scene_1 = new Scene{"scene_1", {light_render_obj, sphere1, sphere2, sphere3, sphere4}, {}};
    scene_1->set_on_create([&camera, &light, &light_render_obj]() {
        camera->set_position(glm::vec3(0.f, 0.f, 6.f));
        camera->set_pitch_yaw(0.f, 0.f);
        light->set_position(glm::vec3(0.f, 0.f, 0.f));
        light_render_obj->get_transform()->scale(0.3);
    });

    // scene 2
    auto sun = new RenderObject{model_sphere, shader_constant, material_yellow};

    auto mars_pivot = new TransformComposite{};
    sun->set_transform(sun->get_transform()->attach(mars_pivot));
    auto mars = new RenderObject{model_sphere, shader_blinn, material_red};
    mars_pivot->attach(mars->get_transform());

    auto earth_pivot = new TransformComposite{};
    sun->set_transform(sun->get_transform()->attach(earth_pivot));
    auto earth = new RenderObject{model_sphere, shader_blinn, material_blue};
    earth_pivot->attach(earth->get_transform());

    auto moon_pivot = new TransformComposite{};
    earth->set_transform(earth->get_transform()->attach(moon_pivot));
    auto moon = new RenderObject{model_sphere, shader_blinn, material_grey};
    moon_pivot->attach(moon->get_transform());

    mars->get_transform()->set_position(glm::vec3(0.f, 4.f, 0.f));
    mars->get_transform()->scale(0.6f);

    earth->get_transform()->set_position(glm::vec3(3.f, 0.f, 0.f));
    earth->get_transform()->scale(0.5f);

    moon->get_transform()->set_position(glm::vec3(2.f, 0.f, 0.f));
    moon->get_transform()->scale(0.5f);

    auto scene_2 = new Scene{"scene_2", {light_render_obj, sun, mars, earth, moon},
                             {earth_pivot, moon_pivot, mars_pivot}};
    scene_2->set_on_create([&light]() {
        light->set_position(glm::vec3(0.f, 0.f, 0.f));
    });
    scene_2->set_on_update([&mars_pivot, &moon_pivot, &earth_pivot]() {
        mars_pivot->rotate(glm::vec3(0.f, 0.f, 1.f));
        earth_pivot->rotate(glm::vec3(0.f, 1.f, 0.f));
        moon_pivot->rotate(glm::vec3(0.f, 0.f, 1.f));
    });

    // scene 3
    auto sphere_scene_3 = new RenderObject{model_sphere, shader_phong, material_blue};
    auto scene_3 = new Scene{"scene_3", {light_render_obj, sphere_scene_3}, {}};
    scene_3->set_on_create([&camera, &light, &light_render_obj, &sphere_scene_3]() {
        camera->set_position(glm::vec3(0.f, -5.f, 0.f));
        camera->set_pitch_yaw(90.f, 0.f);
        light->set_position(glm::vec3(0.f, 5.f, 0.f));
        light_render_obj->get_transform()->scale(0.3);
        sphere_scene_3->get_material()->set_shininess(1.f);
    });

    // scene 5
    auto render_objects = std::vector<RenderObject *>{light_render_obj};

    auto plain = new RenderObject{model_plain, shader_blinn, material_green};
    render_objects.push_back(plain);
    plain->get_transform()->scale(1000.f);

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

    auto scene_5 = new Scene{"scene_5", render_objects, {}};
    scene_5->set_on_create([&camera, &light, &light_render_obj, &render_objects]() {
        camera->set_position(glm::vec3(0.f, 0.f, 6.f));
        camera->set_pitch_yaw(0.f, 0.f);
        light->set_position(glm::vec3(0.f, 10.f, 0.f));
        light_render_obj->get_transform()->scale(0.3);
    });

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

    scene_5->play();
}
