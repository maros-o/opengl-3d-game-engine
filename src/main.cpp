#include "OpenGLContext.h"
#include "Model.h"
#include "RenderObject.h"
#include "VAO.h"
#include "Camera.h"
#include "Light/PointLight.h"
#include "Light/DirectionalLight.h"
#include "InputManager.h"
#include "Scene.h"
#include "../include/Transform/TransformComposite.h"
#include "../res/vertices.h"
#include "../res/indices.h"
#include "../res/models/float_vectors/plain.h"
#include "../res/models/float_vectors/plain_texture.h"
#include "../res/models/float_vectors/suzi_flat.h"
#include "../res/models/float_vectors/suzi_smooth.h"
#include "../res/models/float_vectors/sphere.h"
#include "../res/models/float_vectors/gift.h"
#include "../res/models/float_vectors/bushes.h"
#include "../res/models/float_vectors/tree.h"
#include "../res/models/float_vectors/sky_cube.h"
#include "Light/SpotLight.h"
#include "ModelLoader.h"
#include "Utils.h"
#include "motions/LineMotion.h"
#include "motions/BezierMotion.h"

int main() {
    OpenGLContext &context = OpenGLContext::get_instance().init(2880, 1800, "ZPG - MEC0045");

    InputManager &input_manager = InputManager::get_instance().init();

    auto camera = new Camera(context.get_screen_width(), context.get_screen_height());

    auto light_1 = new PointLight();
    auto light_2 = new PointLight();
    auto dir_light_1 = new DirectionalLight();
    dir_light_1->set_strength(0.4f);
    dir_light_1->set_direction(glm::vec3(0.5f, -1.f, 0.5f));

    auto spot_light_1 = new SpotLight();
    spot_light_1->get_transform()->set_position(glm::vec3(0.f, 0.f, -1.f));
    camera->get_transform_mount()->attach(spot_light_1->get_transform());
    camera->subscribe(spot_light_1);
    spot_light_1->set_constant_strength(1.f);

    // shaders
    auto shader_constant = new ShaderProgram{"../shaders/constant_pos.glsl", camera};
    auto shader_constant_texture = new ShaderProgram{"../shaders/constant_texturecube_pos.glsl", camera};
    auto shader_blinn = new ShaderProgram{"../shaders/blinn_pos_nor.glsl", camera,
                                          {spot_light_1, dir_light_1, light_2}};
    auto shader_blinn_texture = new ShaderProgram{"../shaders/blinn_texture_pos_nor_tep.glsl", camera,
                                                  {spot_light_1, dir_light_1, light_2}};

    // vaos
    auto vao_sphere = new VAO(VBO{sphere, 6});
    auto vao_suzi = new VAO(VBO{suzi_smooth, 6});
    auto vao_plain = new VAO(VBO{plain, 6});
    auto vao_bushes = new VAO(VBO{bushes, 6});
    auto vao_tree = new VAO(VBO{tree, 6});
    auto vao_gift = new VAO(VBO{gift, 6});
    auto vao_plain_texture = new VAO(VBO{plain_texture, 8});
    auto vao_sky_cube_texture = new VAO(VBO{sky_cube, 3});
    {
        vao_sphere->link_attributes({3, 3});
        vao_suzi->link_attributes({3, 3});
        vao_plain->link_attributes({3, 3});
        vao_bushes->link_attributes({3, 3});
        vao_tree->link_attributes({3, 3});
        vao_gift->link_attributes({3, 3});
        vao_plain_texture->link_attributes({3, 3, 2});
        vao_sky_cube_texture->link_attributes({3});
    }

    // models
    auto model_sphere = new Model{"model_sphere", vao_sphere};
    auto model_plain = new Model{"model_plain", vao_plain_texture};
    auto model_sky_cube = new Model{"model_sky_cube", vao_sky_cube_texture};
    auto models_tree = ModelLoader::load_models("../res/models/objs/tree/tree.obj");
    auto models_tree2 = ModelLoader::load_models("../res/models/objs/tree2/tree.obj");
    auto models_zombie = ModelLoader::load_models("../res/models/objs/zombie/zombie.obj");
    auto models_building = ModelLoader::load_models("../res/models/objs/building/building.obj");
    auto models_teren = ModelLoader::load_models("../res/models/objs/teren.obj");

    // materials
    Material material_yellow{glm::vec3(255.f, 255.f, 0.f) / 255.f};
    Material material_blue{glm::vec3(20.f, 20.f, 255.f) / 255.f};
    Material material_green{glm::vec3(25.f, 117.f, 25.f) / 255.f};
    Material material_grey{glm::vec3(100.f, 100.f, 100.f) / 255.f};
    Material material_red{glm::vec3(255.f, 25.f, 25.f) / 255.f};

    // objects
    auto render_objects = std::vector<RenderObject *>{};
    auto objectless_transforms = std::vector<Transform *>{camera->get_transform_mount()};

    auto zombie = new RenderObject(models_zombie[0], shader_blinn_texture,
                                   Material{new Texture{"../res/models/objs/zombie/zombie.png"}});
    render_objects.push_back(zombie);
    zombie->get_transform()->scale(3.f);
    zombie->get_transform()->set_position(glm::vec3(0.f, 0.f, 10.f));
    zombie->get_transform()->set_rotation(glm::vec3(0.f, 180.f, 0.f));

    auto building = new RenderObject(models_building[0], shader_blinn_texture,
                                     Material{new Texture{"../res/models/objs/building/building.png"}});
    render_objects.push_back(building);
    building->get_transform()->scale(3.f);
    building->get_transform()->set_position(glm::vec3(0.f, 0.f, 50.f));

    for (int i = 0; i < 20; i++) {
        auto tree_obj = new RenderObject{models_tree2[0],
                                         shader_blinn_texture,
                                         Material{new Texture{
                                                 "../res/models/objs/tree2/tree.png"}}};
        render_objects.push_back(tree_obj);

        tree_obj->get_transform()->scale(Utils::random_float(0.3f, 0.7f));
        tree_obj->get_transform()->set_position(
                glm::vec3(Utils::random_float(-100.f, 100.f), 0.f, Utils::random_float(-100.f, 20.f)));
        tree_obj->get_transform()->rotate(glm::vec3(0.f, Utils::random_float(0.f, 360.f), 0.f));
    }

    auto light_2_render_obj = new RenderObject{model_sphere, shader_constant, material_yellow};
    render_objects.push_back(light_2_render_obj);
    light_2->get_transform()->attach(light_2_render_obj->get_transform());

    auto floor = new RenderObject{models_teren[0], shader_blinn_texture,
                                  Material{new Texture{"../res/textures/grass.png", 5}}};
    render_objects.push_back(floor);
    floor->get_transform()->scale(8.f);
    floor->get_material().specular_strength = 0.f;

    auto sky_box = new RenderObject{model_sky_cube, shader_constant_texture,
                                    Material{new Texture{{"../res/textures/skybox/posx.jpg",
                                                          "../res/textures/skybox/negx.jpg",
                                                          "../res/textures/skybox/posy.jpg",
                                                          "../res/textures/skybox/negy.jpg",
                                                          "../res/textures/skybox/posz.jpg",
                                                          "../res/textures/skybox/negz.jpg"}}}};
    sky_box->get_transform()->scale(10.f);

    auto scene = new Scene{"forest", render_objects, objectless_transforms};
    scene->set_on_create([&camera, &spot_light_1, &light_2]() {
        camera->set_position(glm::vec3(0.f, 5.f, 0.f));
        spot_light_1->set_position(glm::vec3(0.f, 0.f, -1.f));
        light_2->set_position(glm::vec3(30.f, 20.f, 20.f));
    });
    scene->add_on_update([&camera, &sky_box]() {
        auto sky_box_pos = camera->get_position();
        sky_box_pos.y += 3.f;
        sky_box->get_transform()->set_position(sky_box_pos);
    });
    scene->set_sky_box(sky_box);

    std::vector<glm::vec3> bezier_points;
    BezierMotion *bezier_motion = nullptr;

    // inputs
    {
        input_manager.register_key_press_callback(GLFW_MOUSE_BUTTON_RIGHT,
                                                  [&input_manager, &camera, &scene, &shader_constant, &model_sphere, &material_red, &bezier_points]() {
                                                      int mouse_x = input_manager.get_mouse_x();
                                                      int mouse_y = input_manager.get_mouse_y();
                                                      GLfloat depth = OpenGLContext::get_pixel_depth(mouse_x, mouse_y);
                                                      GLuint index = OpenGLContext::get_pixel_stencil(mouse_x, mouse_y);
                                                      printf("Clicked on pixel: (%d, %d), depth: %f, stencil index: %u\n",
                                                             mouse_x, mouse_y, depth, index);
                                                      glm::vec3 pixel_world_pos = OpenGLContext::get_pixel_world_position(
                                                              mouse_x,
                                                              mouse_y,
                                                              camera);

                                                      bezier_points.emplace_back(pixel_world_pos);

                                                      auto point_obj = new RenderObject{model_sphere,
                                                                                        shader_constant, material_red};
                                                      point_obj->get_transform()->scale(3.f);
                                                      point_obj->get_transform()->set_position(pixel_world_pos);
                                                      scene->add_render_object(point_obj);
                                                  });

        input_manager.register_key_press_callback(GLFW_KEY_P,
                                                  [&scene, &bezier_motion, &bezier_points, &model_sphere, &material_blue, &shader_constant]() {
                                                      if (bezier_points.size() % 4 != 0) {
                                                          printf("bezier_points.size() % 4 != 0");
                                                          return;
                                                      }
                                                      bezier_motion = new BezierMotion(bezier_points);
                                                      scene->add_transform(bezier_motion->get_transform_mount());

                                                      auto curve_obj = new RenderObject{model_sphere, shader_constant,
                                                                                        material_blue};
                                                      curve_obj->get_transform()->scale(5.f);
                                                      scene->add_render_object(curve_obj);

                                                      bezier_motion->get_transform_mount()->attach(
                                                              curve_obj->get_transform());

                                                      scene->add_on_update([&bezier_motion]() {
                                                          bezier_motion->move();
                                                      });
                                                  });

        input_manager.register_key_press_callback(GLFW_KEY_C,
                                                  [&input_manager, &scene]() {
                                                      GLuint id = OpenGLContext::get_pixel_stencil(
                                                              input_manager.get_mouse_x(), input_manager.get_mouse_y());
                                                      printf("Removing object with id: %u\n", id);
                                                      scene->remove_object_by_id(id);
                                                  });

        input_manager.register_key_down_callback(GLFW_KEY_UP, [&light_2]() {
            light_2->move(glm::vec3(0.0f, 0.0f, .1f));
        });
        input_manager.register_key_down_callback(GLFW_KEY_DOWN, [&light_2]() {
            light_2->move(glm::vec3(0.0f, 0.0f, -.1f));
        });
        input_manager.register_key_down_callback(GLFW_KEY_LEFT, [&light_2]() {
            light_2->move(glm::vec3(-.1f, 0.0f, 0.0f));
        });
        input_manager.register_key_down_callback(GLFW_KEY_RIGHT, [&light_2]() {
            light_2->move(glm::vec3(.1f, 0.0f, 0.0f));
        });
        input_manager.register_key_down_callback(GLFW_KEY_PAGE_UP, [&light_2]() {
            light_2->move(glm::vec3(0.0f, .1f, 0.0f));
        });
        input_manager.register_key_down_callback(GLFW_KEY_PAGE_DOWN, [&light_2]() {
            light_2->move(glm::vec3(0.0f, -.1f, 0.0f));
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
                    if (input_manager.is_key_down(GLFW_MOUSE_BUTTON_LEFT)) {
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
