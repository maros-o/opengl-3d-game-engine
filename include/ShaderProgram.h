#pragma once

#include <GL/glew.h>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>
#include "CameraObserver.h"
#include "Camera.h"

class ShaderProgram : public CameraObserver {
public:
    ShaderProgram(const char *shader_file_path, Camera *camera);

    ~ShaderProgram();

    void use() const;

    static void reset();

    void set_uniform_1i(const char *name, int value);

    void set_uniform_vec3f(const char *name, const glm::vec3 &vector);

    void set_uniform_vec4f(const char *name, const glm::vec4 &vector);

    void set_uniform_mat4f(const char *name, const glm::mat4 &matrix);

    void print_active_uniforms() const;

    void set_camera(Camera *camera);

    void view_matrix_changed() final;

    void projection_matrix_changed() final;

private:
    GLuint id = 0;
    std::unordered_map<const char *, GLint> uniform_locations;

    Camera *camera = nullptr;

    void destroy() const;

    GLint get_uniform_location(const char *name);
};
