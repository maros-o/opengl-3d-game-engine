#include <random>
#include <assimp/Importer.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/vec4.hpp>

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

static float random_normalized_float() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    return dis(gen);
}


struct Vertex {
    float Position[3];
    float Normal[3];
    float Texture[2];
    float Tangent[3];
};

VAO *load(const std::string &fileName) {
    Assimp::Importer importer;
    unsigned int importOptions = aiProcess_Triangulate
                                 | aiProcess_OptimizeMeshes
                                 | aiProcess_JoinIdenticalVertices
                                 | aiProcess_CalcTangentSpace;

    // aiProcess_GenNormals/ai_Process_GenSmoothNormals - vypocet normal s jemnych prechodem v pripade, ze objekt neobsahuje normaly

    const aiScene *scene = importer.ReadFile(fileName, importOptions);

    if (scene) {
        printf("scene->mNumMeshes = %d\n", scene->mNumMeshes);
        printf("scene->mNumMaterials = %d\n", scene->mNumMaterials);

        for (unsigned int i = 0; i < scene->mNumMaterials; i++)                       //Materials
        {
            const aiMaterial *mat = scene->mMaterials[i];

            aiString name;
            mat->Get(AI_MATKEY_NAME, name);
            printf("Material [%d] name %s\n", i, name.C_Str());

            aiColor4D d;

            glm::vec4 diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
            if (AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &d))
                diffuse = glm::vec4(d.r, d.g, d.b, d.a);

        }

        for (unsigned int i = 0; i < scene->mNumMeshes; i++)                      //Objects
        {
            aiMesh *mesh = scene->mMeshes[i];

            auto *pVertices = new Vertex[mesh->mNumVertices];
            std::memset(pVertices, 0, sizeof(Vertex) * mesh->mNumVertices);

            for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
                if (mesh->HasPositions()) {
                    pVertices[i].Position[0] = mesh->mVertices[i].x;
                    pVertices[i].Position[1] = mesh->mVertices[i].y;
                    pVertices[i].Position[2] = mesh->mVertices[i].z;
                }
                if (mesh->HasNormals()) {
                    pVertices[i].Normal[0] = mesh->mNormals[i].x;
                    pVertices[i].Normal[1] = mesh->mNormals[i].y;
                    pVertices[i].Normal[2] = mesh->mNormals[i].z;
                }
                if (mesh->HasTextureCoords(0)) {
                    pVertices[i].Texture[0] = mesh->mTextureCoords[0][i].x;
                    pVertices[i].Texture[1] = mesh->mTextureCoords[0][i].y;
                }
                if (mesh->HasTangentsAndBitangents()) {
                    pVertices[i].Tangent[0] = mesh->mTangents[i].x;
                    pVertices[i].Tangent[1] = mesh->mTangents[i].y;
                    pVertices[i].Tangent[2] = mesh->mTangents[i].z;
                }

            }

            std::vector<GLuint> ebo_data;

            if (mesh->HasFaces()) {
                for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
                    ebo_data.push_back(mesh->mFaces[i].mIndices[0]);
                    ebo_data.push_back(mesh->mFaces[i].mIndices[1]);
                    ebo_data.push_back(mesh->mFaces[i].mIndices[2]);
                }
            }

            std::vector<GLfloat> vbo_data = {};

            for (int i = 0; i < mesh->mNumVertices; i++) {
                vbo_data.push_back(pVertices[i].Position[0]);
                vbo_data.push_back(pVertices[i].Position[1]);
                vbo_data.push_back(pVertices[i].Position[2]);

                vbo_data.push_back(pVertices[i].Normal[0]);
                vbo_data.push_back(pVertices[i].Normal[1]);
                vbo_data.push_back(pVertices[i].Normal[2]);

                vbo_data.push_back(pVertices[i].Texture[0]);
                vbo_data.push_back(pVertices[i].Texture[1]);

                vbo_data.push_back(pVertices[i].Tangent[0]);
                vbo_data.push_back(pVertices[i].Tangent[1]);
                vbo_data.push_back(pVertices[i].Tangent[2]);
            }

            auto vao = new VAO{VBO{vbo_data, sizeof(Vertex)}, EBO{ebo_data}};
            vao->link_attributes(0, 3, GL_FLOAT, sizeof(Vertex), nullptr);
            vao->link_attributes(1, 3, GL_FLOAT, sizeof(Vertex), (void *) (3 * sizeof(GLfloat)));
            vao->link_attributes(2, 2, GL_FLOAT, sizeof(Vertex), (void *) (6 * sizeof(GLfloat)));
            vao->link_attributes(3, 3, GL_FLOAT, sizeof(Vertex), (void *) (8 * sizeof(GLfloat)));

            delete[] pVertices;

            return vao;
        }
    } else {
        printf("Error during parsing mesh from %s : %s \n", fileName.c_str(), importer.GetErrorString());
    }
    glBindVertexArray(0);
}

int main() {
    OpenGLContext &context = OpenGLContext::get_instance().init(1920, 1200, "ZPG - MEC0045");

    InputManager &input_manager = InputManager::get_instance().init();

    auto camera = new Camera(context.get_screen_width(), context.get_screen_height());

    auto light_1 = new PointLight();
    auto light_2 = new PointLight();
    auto dir_light_1 = new DirectionalLight();
    dir_light_1->set_strength(0.8f);
    auto spot_light_1 = new SpotLight();
    spot_light_1->set_cut_off(0.92f);
    spot_light_1->set_constant_strength(1.f);

    // shaders
    auto shader_constant = new ShaderProgram{"../shaders/constant_pos.glsl", camera};
    auto shader_constant_texture = new ShaderProgram{"../shaders/constant_texture_pos.glsl", camera};
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
    auto vao_zombie = load("../res/models/objs/mercedes.obj");
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

        vao_plain_texture->link_attributes(0, 3, GL_FLOAT, 8 * sizeof(GLfloat), nullptr);
        vao_plain_texture->link_attributes(1, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
        vao_plain_texture->link_attributes(2, 2, GL_FLOAT, 8 * sizeof(GLfloat), (void *) (6 * sizeof(GLfloat)));

        vao_sky_cube_texture->link_attributes(0, 3, GL_FLOAT, 3 * sizeof(GLfloat), nullptr);
    }

    // materials
    auto material_yellow = new Material{glm::vec3(255.f, 255.f, 0.f) / 255.f};
    auto material_blue = new Material{glm::vec3(98.f, 98.f, 255.f) / 255.f};
    auto material_green = new Material{glm::vec3(25.f, 117.f, 25.f) / 255.f};
    auto material_grey = new Material{glm::vec3(100.f, 100.f, 100.f) / 255.f};
    auto material_red = new Material{glm::vec3(255.f, 25.f, 25.f) / 255.f};
    auto material_plain_texture = new Material{new Texture{"../res/textures/grass.png"}};
    auto material_zombie_texture = new Material{new Texture{"../res/models/objs/building.png"}};
    auto material_sky_cube_texture = new Material{new Texture{{"../res/textures/skybox/posx.jpg",
                                                               "../res/textures/skybox/negx.jpg",
                                                               "../res/textures/skybox/posy.jpg",
                                                               "../res/textures/skybox/negy.jpg",
                                                               "../res/textures/skybox/posz.jpg",
                                                               "../res/textures/skybox/negz.jpg"}}};

    // models
    auto model_sphere = new Model{"model_sphere", vao_sphere};
    auto model_tree = new Model{"model_tree", vao_tree};
    auto model_plain = new Model{"model_plain", vao_plain_texture};
    auto model_bushes = new Model{"model_bushes", vao_bushes};
    auto model_gift = new Model{"model_gift", vao_gift};
    auto model_sky_cube = new Model{"model_sky_cube", vao_sky_cube_texture};
    auto model_zombie = new Model{"model_zombie", vao_zombie};


    //auto *light_1_render_obj = new RenderObject{model_gift, shader_constant, material_yellow};
    //spot_light_1->get_transform()->attach(light_1_render_obj->get_transform());

    spot_light_1->set_camera(camera);

    auto *light_2_render_obj = new RenderObject{model_sphere, shader_constant, material_yellow};
    light_2->get_transform()->attach(light_2_render_obj->get_transform());

    auto zombie = new RenderObject(model_zombie, shader_blinn_texture, material_zombie_texture);

    auto render_objects = std::vector<RenderObject *>{light_2_render_obj};
    render_objects.push_back(zombie);
    zombie->get_transform()->scale(5.f);

    auto floor = new RenderObject{model_plain, shader_blinn_texture, material_plain_texture};
    render_objects.push_back(floor);
    floor->get_transform()->scale(30.f);
    floor->get_material()->set_specular_strength(0.f);

    auto sky_box = new RenderObject{model_sky_cube, shader_constant_texture, material_sky_cube_texture};
    sky_box->get_transform()->scale(10.f);

    float max_x = 30.f;
    float max_z = 30.f;

    // trees
    for (int x = 0; x < 10; x++) {
        for (int z = 0; z < 10; z++) {
            auto obj = new RenderObject{model_tree, shader_blinn, material_blue};
            obj->get_transform()->set_position(glm::vec3(max_x * (random_normalized_float() - 1.f),
                                                         0.0f,
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
                                                         0.0f,
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
                                                         0.0f,
                                                         max_z * (random_normalized_float() - 1.f)));
            render_objects.push_back(obj);
        }
    }

    auto scene = new Scene{"forest", render_objects,
                           {camera->get_transform_mount()}};
    scene->set_on_create([&camera, &spot_light_1, &light_2, &light_1]() {
        camera->set_position(glm::vec3(0.f, 5.f, 0.f));
        spot_light_1->set_position(glm::vec3(0.f, 0.f, -1.f));
        light_2->set_position(glm::vec3(20.f, 10.f, 20.f));
    });
    scene->set_on_update([&camera, &sky_box]() {
        sky_box->get_transform()->set_position(camera->get_position());
    });
    scene->set_sky_box(sky_box);


    // inputs
    {
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
