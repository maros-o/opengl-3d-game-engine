#include "OpenGLContext.h"
#include "Renderer.h"
#include "EBO.h"
#include "Engine.h"
#include "Model.h"
#include "../res/vertices.h"
#include "../res/indices.h"
#include "OrthographicCamera.h"

int main() {
    auto context = new OpenGLContext(900, 600, "ZPG - MEC0045");

//    auto shader_program_rgb = new ShaderProgram{"../res/shaders/rgb.shader"};
//    auto shader_program_red = new ShaderProgram{"../res/shaders/red.shader"};
//    auto shader_program_blue = new ShaderProgram{"../res/shaders/blue.shader"};
//
//    auto vbo_triangle = new VBO{vertices_triangle_rgb, sizeof(vertices_triangle_rgb)};
//    auto ebo_triangle = new EBO{indices_triangle, sizeof(indices_triangle)};
//    auto vao_triangle = new VAO{vbo_triangle, ebo_triangle};
//    vao_triangle->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
//    auto *triangle_model = new Model{"triangle", vao_triangle, shader_program_red};

//    auto vbo_cube = new VBO{vertices_cube_rgb, sizeof(vertices_cube_rgb)};
//    auto ebo_cube = new EBO{indices_cube, sizeof(indices_cube)};
//    auto vao_cube = new VAO{vbo_cube, ebo_cube};
//    vao_cube->link_attributes(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
//    vao_cube->link_attributes(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
//    auto *cube_model = new Model{"cube", vao_cube, shader_program_rgb};

    auto vbo_square = new VBO{vertices_cube_texture, sizeof(vertices_cube_texture)};
    auto ebo_square = new EBO{indices_cube, sizeof(indices_cube)};
    auto vao_square = new VAO{vbo_square, ebo_square};
    vao_square->link_attributes(0, 3, GL_FLOAT, 5 * sizeof(GLfloat), nullptr);
    vao_square->link_attributes(1, 2, GL_FLOAT, 5 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    auto *square_model = new Model{"square", vao_square, new ShaderProgram{"../res/shaders/texture.shader"},
                                   new Texture{"../res/textures/fei.png"}};

    auto square_object = new RenderObject{square_model};

    // auto triangle_object = new RenderObject{triangle_model};
    //auto cube_object = new RenderObject{cube_model};

    auto renderer = new Renderer();
    // renderer->add_object(triangle_object);
    //renderer->add_object(cube_object);
    renderer->add_object(square_object);

    auto camera = new OrthographicCamera(context->get_width(), context->get_height());

    Engine::run(context, camera, renderer);
}
