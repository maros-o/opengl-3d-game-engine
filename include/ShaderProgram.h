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

enum class ShaderUniform {
    MODEL_MATRIX,
    VIEW_MATRIX,
    PROJECTION_MATRIX,
    NORMAL_MATRIX,
    TEXTURE_SAMPLER,
    CAMERA_WORLD_POSITION,
    LIGHT_WORLD_POSITION,
    LIGHT_COLOR,
    LIGHT_CONSTANT,
    LIGHT_LINEAR,
    LIGHT_QUADRATIC,
    OBJECT_COLOR,
    OBJECT_AMBIENT,
    OBJECT_DIFFUSE,
    OBJECT_SPECULAR,
    OBJECT_SHININESS,
};

class ShaderProgram : public Observer {
public:
    ShaderProgram(const char *shader_file_path, Camera *camera);

    ShaderProgram(const char *shader_file_path, Camera *camera, Light *light);

    ~ShaderProgram();

    void use() const;

    static void reset();

    void set_uniform(ShaderUniform uniform, float value);

    void set_uniform(ShaderUniform uniform, const glm::vec3 &vector);

    void set_uniform(ShaderUniform uniform, const glm::mat4 &matrix);

    void set_camera(Camera *camera);

    void set_light(Light *light);

    void update(int event) override;

private:
    GLuint id = 0;
    std::unordered_map<std::string, GLint> uniform_locations;

    Camera *camera = nullptr;

    Light *light = nullptr;

    void destroy() const;

    void init(const char *shader_file_path);

    GLint get_uniform_location(ShaderUniform uniform);
};