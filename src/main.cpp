#include "OpenGLContext.h"
#include "Renderer.h"
#include "buffers/EBO.h"
#include "Engine.h"
#include "Model.h"
#include "../res/vertices.h"
#include "../res/indices.h"
#include "Camera.h"
#include "InputManager.h"
#include <assimp/Importer.hpp>
#include "../res/3d_models/plain.h"
#include "../res/3d_models/suzi_flat.h"
#include "../res/3d_models/suzi_smooth.h"
#include "../res/3d_models/sphere.h"
#include "../res/3d_models/gift.h"
#include "../res/3d_models/bushes.h"
#include "../res/3d_models/tree.h"


int main() {
    OpenGLContext &context = OpenGLContext::get_instance().init(900, 600, "ZPG - MEC0045");

    InputManager &input_manager = InputManager::get_instance().init();

    auto camera = new Camera(context.get_screen_width(), context.get_screen_height());


    auto shader_program_texture = new ShaderProgram{"../res/shaders/texture.shader", camera};
    auto shader_program_rgb = new ShaderProgram{"../res/shaders/rgb.shader", camera};
    auto shader_program_red = new ShaderProgram{"../res/shaders/red.shader", camera};
    auto shader_program_yellow = new ShaderProgram{"../res/shaders/yellow.shader", camera};
    auto shader_program_green = new ShaderProgram{"../res/shaders/green.shader", camera};

    auto vbo_tree = new VBO{tree, 6};
    auto vao_tree = new VAO(vbo_tree);
    vao_tree->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
    auto tree_model = new Model{"green_tree", vao_tree, shader_program_green};

    auto vbo_bushes = new VBO{bushes, 6};
    auto vao_bushes = new VAO(vbo_bushes);
    vao_bushes->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
    auto bushes_model = new Model{"yellow_bushes", vao_bushes, shader_program_yellow};

    auto vbo_gift = new VBO{gift, 6};
    auto vao_gift = new VAO(vbo_gift);
    vao_gift->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
    auto gift_model = new Model{"red_gift", vao_gift, shader_program_red};

    auto vbo_suzi = new VBO{suzi_smooth, 6};
    auto vao_suzi = new VAO(vbo_suzi);
    vao_suzi->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
    auto suzi_model = new Model{"red_suzi", vao_suzi, shader_program_red};

    auto vbo_sphere = new VBO{sphere, 6};
    auto vao_sphere = new VAO(vbo_sphere);
    vao_sphere->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
    auto sphere_model = new Model{"yellow_sphere", vao_sphere, shader_program_yellow};

    auto vbo_triangle = new VBO{vertices_triangle_rgb, 6};
    auto vao_triangle = new VAO{vbo_triangle, new EBO{indices_triangle}};
    vao_triangle->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
    vao_triangle->link_attributes(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    auto triangle_model = new Model{"rgb_triangle", vao_triangle, shader_program_rgb};

    auto vbo_fei_cube = new VBO{vertices_cube_texture, 6};
    auto vao_fei_cube = new VAO{vbo_fei_cube, new EBO{indices_cube}};
    vao_fei_cube->link_attributes(0, 3, GL_FLOAT, 5 * sizeof(GLfloat), nullptr);
    vao_fei_cube->link_attributes(1, 2, GL_FLOAT, 5 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    auto fei_cube_model = new Model{"fei_cube", vao_fei_cube, shader_program_texture,
                                    new Texture{"../res/textures/fei.png"}};


    RenderObject *triangle_1 = new RenderObjectLeaf{triangle_model};
    RenderObject *triangle_2 = new RenderObjectLeaf{triangle_model};
    RenderObject *fei_cube_1 = new RenderObjectLeaf{fei_cube_model};
    RenderObject *fei_cube_2 = new RenderObjectLeaf{fei_cube_model};
    RenderObject *suzi_1 = new RenderObjectLeaf(suzi_model);
    RenderObject *sphere_1 = new RenderObjectLeaf(sphere_model);
    RenderObject *gift_1 = new RenderObjectLeaf(gift_model);
    RenderObject *bushes_1 = new RenderObjectLeaf(bushes_model);
    RenderObject *tree_1 = new RenderObjectLeaf(tree_model);

    suzi_1 = suzi_1->attach(fei_cube_1);
    suzi_1 = suzi_1->attach(sphere_1);
    fei_cube_1 = fei_cube_1->attach(gift_1);

    auto renderer = new Renderer();
    renderer->add_object(fei_cube_1);
    renderer->add_object(triangle_1);
    renderer->add_object(triangle_2);
    renderer->add_object(fei_cube_2);
    renderer->add_object(suzi_1);
    renderer->add_object(sphere_1);
    renderer->add_object(gift_1);
    renderer->add_object(bushes_1);
    renderer->add_object(tree_1);

    triangle_1->translate(glm::vec3(1.f, 0.0f, 0.0f));
    triangle_2->translate(glm::vec3(-1.f, 0.0f, 0.0f));
    suzi_1->translate(glm::vec3(0.0f, 2.0f, 0.0f))->scale(0.6f);
    sphere_1->translate(glm::vec3(0.0f, -2.0f, 0.0f));
    gift_1->translate(glm::vec3(3.0f, .0f, 0.0f));
    bushes_1->translate(glm::vec3(-3.0f, .0f, 0.0f));
    tree_1->translate(glm::vec3(-10.0f, -5.0f, 0.0f));
    fei_cube_1->translate(glm::vec3(2.0f, 0.0f, 0.0f))->scale(2);


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

    input_manager.register_key_down_callback(GLFW_KEY_UP, [&suzi_1]() {
        suzi_1->rotate(glm::vec3(3.0f, 0.0f, 0.0f));
    });
    input_manager.register_key_down_callback(GLFW_KEY_DOWN, [&suzi_1]() {
        suzi_1->rotate(glm::vec3(-3.0f, 0.0f, 0.0f));
    });
    input_manager.register_key_down_callback(GLFW_KEY_LEFT, [&suzi_1]() {
        suzi_1->rotate(glm::vec3(0.0f, 3.0f, 0.0f));
    });
    input_manager.register_key_down_callback(GLFW_KEY_RIGHT, [&suzi_1]() {
        suzi_1->rotate(glm::vec3(0.0f, -3.0f, 0.0f));
    });
    input_manager.register_key_down_callback(GLFW_KEY_4, [&suzi_1]() {
        suzi_1->translate(glm::vec3(0.0f, 0.0f, -0.1f));
    });
    input_manager.register_key_down_callback(GLFW_KEY_5, [&suzi_1]() {
        suzi_1->translate(glm::vec3(0.0f, 0.0f, 0.1f));
    });
    input_manager.register_key_down_callback(GLFW_KEY_7, [&fei_cube_1]() {
        fei_cube_1->translate(glm::vec3(0.0f, 0.0f, -0.1f))->rotate(glm::vec3(0.0f, 0.0f, 3.0f));
    });
    input_manager.register_key_down_callback(GLFW_KEY_8, [&fei_cube_1]() {
        fei_cube_1->translate(glm::vec3(0.0f, 0.0f, 0.1f))->rotate(glm::vec3(0.0f, 0.0f, -3.0f));
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
