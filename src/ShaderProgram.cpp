#include <fstream>
#include <string>
#include <sstream>

#include "ShaderProgram.h"
#include "buffers/SSBO.h"
#include "Light/DirectionalLight.h"
#include "Light/SpotLight.h"

static std::unordered_map<ShaderUniform, std::string> ShaderUniformToString = {
        {ShaderUniform::MODEL_MATRIX,          "u_model_matrix"},
        {ShaderUniform::VIEW_MATRIX,           "u_view_matrix"},
        {ShaderUniform::PROJECTION_MATRIX,     "u_projection_matrix"},
        {ShaderUniform::NORMAL_MATRIX,         "u_normal_matrix"},
        {ShaderUniform::TEXTURE_SAMPLER,       "u_texture_sampler"},
        {ShaderUniform::TEXTURE_REPEAT,        "u_texture_repeat"},
        {ShaderUniform::CAMERA_WORLD_POSITION, "u_camera_world_position"},
        {ShaderUniform::OBJECT_COLOR,          "u_object_color"},
        {ShaderUniform::OBJECT_AMBIENT,        "u_ambient_strength"},
        {ShaderUniform::OBJECT_DIFFUSE,        "u_diffuse_strength"},
        {ShaderUniform::OBJECT_SPECULAR,       "u_specular_strength"},
        {ShaderUniform::OBJECT_SHININESS,      "u_shininess"},
        {ShaderUniform::LIGHT_COUNT,           "u_light_count"},
};

struct ShaderProgramSource {
    char *vertex_source;
    char *fragment_source;
};

static ShaderProgramSource parse_shader(const char *file_path) {
    std::ifstream file_stream(file_path);

    if (!file_stream.is_open()) {
        std::cerr << "ERROR::SHADER::FILE" << std::endl;
        exit(1);
    }

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(file_stream, line)) {
        if (line.find("// #shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else if (type != ShaderType::NONE) {
            ss[(int) type] << line << '\n';
        }
    }

    return {strdup(ss[0].str().c_str()), strdup(ss[1].str().c_str())};
}

static unsigned int compile_shader(GLuint type, const char *source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, nullptr, info_log);
        std::cerr << "ERROR::SHADER::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n"
                  << info_log << std::endl;
        exit(1);
    }

    return shader;
}

void ShaderProgram::init(const char *shader_file_path) {
    ShaderProgramSource source = parse_shader(shader_file_path);

    GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, source.vertex_source);
    if (vertex_shader == 0) {
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl;
        exit(1);
    }
    GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, source.fragment_source);
    if (fragment_shader == 0) {
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl;
        exit(1);
    }

    this->id = glCreateProgram();
    glAttachShader(this->id, fragment_shader);
    glAttachShader(this->id, vertex_shader);
    glLinkProgram(this->id);
    glValidateProgram(this->id);

    GLint success;
    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(this->id, 512, nullptr, info_log);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
        exit(1);
    }

    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);
}

ShaderProgram::ShaderProgram(const char *shader_file_path, Camera *camera) {
    this->init(shader_file_path);
    this->set_camera(camera);
}

ShaderProgram::ShaderProgram(const char *shader_file_path, Camera *camera, std::vector<Light *> lights) {
    this->init(shader_file_path);
    this->set_camera(camera);
    this->set_lights(std::move(lights));
}

ShaderProgram::~ShaderProgram() {
    this->destroy();
}

void ShaderProgram::use() const {
    glUseProgram(this->id);
    this->ssbo_directional_lights.bind_base();
    this->ssbo_point_lights.bind_base();
    this->ssbo_spot_lights.bind_base();
}

void ShaderProgram::reset() {
    glUseProgram(0);
}

void ShaderProgram::destroy() const {
    glDeleteProgram(this->id);
}

GLint ShaderProgram::get_uniform_location(ShaderUniform uniform) {
    std::string name = ShaderUniformToString[uniform];

    if (this->uniform_locations.find(name) == this->uniform_locations.end()) {
        GLint location = glGetUniformLocation(this->id, name.c_str());
        if (location == -1) {
            std::cerr << "ERROR::SHADER::UNIFORM::" << name << "::NOT_FOUND" << std::endl;
        }
        this->uniform_locations[name] = location;
    }
    return this->uniform_locations[name];
}

void ShaderProgram::set_uniform(ShaderUniform uniform, const glm::vec3 &_vec3) {
    glUniform3fv(this->get_uniform_location(uniform), 1, &_vec3[0]);
}

void ShaderProgram::set_uniform(ShaderUniform uniform, const glm::mat4 &_mat4) {
    glUniformMatrix4fv(this->get_uniform_location(uniform), 1, GL_FALSE, &_mat4[0][0]);
}

void ShaderProgram::set_uniform(ShaderUniform uniform, float _float) {
    glUniform1f(this->get_uniform_location(uniform), _float);
}

void ShaderProgram::set_uniform(ShaderUniform uniform, int _int) {
    glUniform1i(this->get_uniform_location(uniform), _int);
}

void ShaderProgram::set_camera(Camera *new_cam) {
    if (this->camera != nullptr) {
        this->camera->unsubscribe(this);
    }
    this->camera = new_cam;
    this->camera->subscribe(this);

    this->use();
    this->set_uniform(ShaderUniform::PROJECTION_MATRIX, this->camera->get_projection_matrix());
    this->set_uniform(ShaderUniform::VIEW_MATRIX, this->camera->get_view_matrix());
    this->set_uniform(ShaderUniform::CAMERA_WORLD_POSITION, this->camera->get_position());
    ShaderProgram::reset();
}

void ShaderProgram::set_lights(std::vector<Light *> new_lights) {
    for (auto light: this->lights) {
        light->unsubscribe(this);
    }
    this->lights = std::move(new_lights);
    for (auto light: this->lights) {
        light->subscribe(this);
    }

    this->update_ssbo_lights();
}

void ShaderProgram::update(int event) {
    this->use();

    switch (event) {
        case (int) CameraEvent::PROJECTION:
            this->set_uniform(ShaderUniform::PROJECTION_MATRIX, this->camera->get_projection_matrix());
            return;
        case (int) CameraEvent::VIEW:
            this->set_uniform(ShaderUniform::VIEW_MATRIX, this->camera->get_view_matrix());
            return;
        case (int) CameraEvent::POSITION:
            this->set_uniform(ShaderUniform::CAMERA_WORLD_POSITION, this->camera->get_position());
            return;
        case (int) LightEvent::ALL:
            this->update_ssbo_lights();
            return;
        default:
            throw std::runtime_error("Unknown event type");
    }
}

struct DirectionalLightSSBO {
    glm::vec4 direction;        // 16
    glm::vec4 color;            // 16
    float strength;             // 4
    float padding_1;            // 4
    float padding_2;            // 4
    float padding_3;            // 4
};                              // 48 bytes

struct PointLightSSBO {
    glm::vec4 world_position;   // 16
    glm::vec4 color;            // 16
    float constant_strength;    // 4
    float linear_strength;      // 4
    float quadratic_strength;   // 4
    float padding_1;            // 4
};                              // 48 bytes

struct SpotLightSSBO {
    glm::vec4 world_position;   // 16
    glm::vec4 color;            // 16
    glm::vec4 direction;        // 16
    float constant_strength;    // 4
    float linear_strength;      // 4
    float quadratic_strength;   // 4
    float cut_off;            // 4
};                              // 64 bytes

void ShaderProgram::update_ssbo_lights() {
    std::vector<DirectionalLightSSBO> directional_lights_ssbo_data;
    std::vector<PointLightSSBO> point_lights_ssbo_data;
    std::vector<SpotLightSSBO> spot_lights_ssbo_data;

    for (auto light: this->lights) {
        if (auto dir_light = dynamic_cast<DirectionalLight *>(light)) {
            directional_lights_ssbo_data.push_back({
                                                           glm::vec4(dir_light->get_direction(), 1.0f),
                                                           glm::vec4(dir_light->get_color(), 1.0f),
                                                           dir_light->get_strength(),
                                                           0.0f,
                                                           0.0f,
                                                           0.0f,
                                                   });
        } else if (auto spot_light = dynamic_cast<SpotLight *>(light)) {
            spot_lights_ssbo_data.push_back({
                                                    glm::vec4(spot_light->get_position(), 1.0f),
                                                    glm::vec4(spot_light->get_color(), 1.0f),
                                                    glm::vec4(spot_light->get_direction(), 1.0f),
                                                    spot_light->get_constant_strength(),
                                                    spot_light->get_linear_strength(),
                                                    spot_light->get_quadratic_strength(),
                                                    spot_light->get_cut_off(),
                                            });
        } else if (auto point_light = dynamic_cast<PointLight *>(light)) {
            point_lights_ssbo_data.push_back({
                                                     glm::vec4(point_light->get_position(), 1.0f),
                                                     glm::vec4(point_light->get_color(), 1.0f),
                                                     point_light->get_constant_strength(),
                                                     point_light->get_linear_strength(),
                                                     point_light->get_quadratic_strength(),
                                                     0.0f,
                                             });
        }
    }

    this->ssbo_directional_lights.allocate_data(directional_lights_ssbo_data.size() * sizeof(DirectionalLightSSBO),
                                                directional_lights_ssbo_data.data());
    this->ssbo_point_lights.allocate_data(point_lights_ssbo_data.size() * sizeof(PointLightSSBO),
                                          point_lights_ssbo_data.data());
    this->ssbo_spot_lights.allocate_data(spot_lights_ssbo_data.size() * sizeof(SpotLightSSBO),
                                         spot_lights_ssbo_data.data());
}
