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
#include "Light/PointLight.h"
#include "buffers/SSBO.h"

enum class ShaderUniform {
    MODEL_MATRIX,
    VIEW_MATRIX,
    PROJECTION_MATRIX,
    NORMAL_MATRIX,
    TEXTURE_SAMPLER,
    CAMERA_WORLD_POSITION,
    OBJECT_COLOR,
    OBJECT_AMBIENT,
    OBJECT_DIFFUSE,
    OBJECT_SPECULAR,
    OBJECT_SHININESS,
    LIGHT_COUNT,
};

class ShaderProgram : public Observer {
public:
    ShaderProgram(const char *shader_file_path, Camera *camera);

    ShaderProgram(const char *shader_file_path, Camera *camera, std::vector<Light *> lights);

    ~ShaderProgram();

    void use() const;

    static void reset();

    void set_uniform(ShaderUniform uniform, int _int);

    void set_uniform(ShaderUniform uniform, float _float);

    void set_uniform(ShaderUniform uniform, const glm::vec3 &_vec3);

    void set_uniform(ShaderUniform uniform, const glm::mat4 &_mat4);

    void set_camera(Camera *camera);

    void set_lights(std::vector<Light *> lights);

    void update(int event) override;

private:
    GLuint id = 0;
    std::unordered_map<std::string, GLint> uniform_locations;

    Camera *camera = nullptr;

    std::vector<Light *> lights;
    SSBO ssbo_directional_lights{0};
    SSBO ssbo_point_lights{1};
    SSBO ssbo_spot_lights{2};

    void destroy() const;

    void init(const char *shader_file_path);

    GLint get_uniform_location(ShaderUniform uniform);

    void update_ssbo_lights();
};