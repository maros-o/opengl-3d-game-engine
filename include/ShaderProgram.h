#pragma once

#include <GL/glew.h>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>
#include <map>

#include "Camera.h"
#include "Light.h"

enum class ShaderUniform;

class ShaderProgram : public IObserver {
public:
    ShaderProgram(const char *shader_file_path, Camera *camera);

    ~ShaderProgram();

    void use() const;

    static void reset();

    void set_uniform_1i(ShaderUniform uniform, int value);

    void set_uniform_1f(ShaderUniform uniform, float value);

    void set_uniform_vec3f(ShaderUniform uniform, const glm::vec3 &vector);

    void set_uniform_vec4f(ShaderUniform uniform, const glm::vec4 &vector);

    void set_uniform_mat4f(ShaderUniform uniform, const glm::mat4 &matrix);

    void print_active_uniforms() const;

    void set_camera(Camera *camera);

    void set_light(Light *light);

    void update(int event) override;

private:
    GLuint id = 0;
    std::unordered_map<std::string, GLint> uniform_locations;

    Camera *camera = nullptr;

    Light *light = nullptr;

    void destroy() const;

    GLint get_uniform_location(ShaderUniform uniform);
};

enum class ShaderUniform {
    MODEL_MATRIX,
    VIEW_MATRIX,
    PROJECTION_MATRIX,
    NORMAL_MATRIX,
    TEXTURE_SAMPLER,
    CAMERA_WORLD_POSITION,
    LIGHT_WORLD_POSITION,
    LIGHT_COLOR,
    LIGHT_AMBIENT_STRENGTH,
    LIGHT_DIFFUSE_STRENGTH,
    LIGHT_SPECULAR_STRENGTH,
};