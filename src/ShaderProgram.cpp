#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

static std::unordered_map<ShaderUniform, std::string> ShaderUniformToString = {
        {ShaderUniform::MODEL_MATRIX,            "u_model_matrix"},
        {ShaderUniform::VIEW_MATRIX,             "u_view_matrix"},
        {ShaderUniform::PROJECTION_MATRIX,       "u_projection_matrix"},
        {ShaderUniform::NORMAL_MATRIX,           "u_normal_matrix"},
        {ShaderUniform::TEXTURE_SAMPLER,         "u_texture_sampler"},
        {ShaderUniform::CAMERA_WORLD_POSITION,   "u_camera_world_position"},
        {ShaderUniform::LIGHT_WORLD_POSITION,    "u_light_world_position"},
        {ShaderUniform::LIGHT_COLOR,             "u_light_color"},
        {ShaderUniform::LIGHT_AMBIENT_STRENGTH,  "u_ambient_strength"},
        {ShaderUniform::LIGHT_DIFFUSE_STRENGTH,  "u_diffuse_strength"},
        {ShaderUniform::LIGHT_SPECULAR_STRENGTH, "u_specular_strength"},
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
        if (line.find("#shader") != std::string::npos) {
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

ShaderProgram::ShaderProgram(const char *shader_file_path, Camera *camera, Light *light) {
    this->init(shader_file_path);
    this->set_camera(camera);
    this->set_light(light);
}

ShaderProgram::~ShaderProgram() {
    this->destroy();
}

void ShaderProgram::use() const {
    glUseProgram(this->id);
}

void ShaderProgram::reset() {
    glUseProgram(0);
}

void ShaderProgram::destroy() const {
    glDeleteProgram(this->id);
}

void ShaderProgram::print_active_uniforms() const {
    GLint i;
    GLint count;

    GLint size;
    GLenum type;

    const GLsizei bufSize = 32;
    GLchar name[bufSize];
    GLsizei length;

    glGetProgramiv(this->id, GL_ACTIVE_UNIFORMS, &count);
    printf("Active Uniforms: %d\n", count);

    for (i = 0; i < count; i++) {
        glGetActiveUniform(this->id, (GLuint) i, bufSize, &length, &size, &type, name);
        printf("Uniform #%d Type: %u, Name: %s, Size: %d\n", i, type, name, size);
    }
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

void ShaderProgram::set_uniform_vec3f(ShaderUniform uniform, const glm::vec3 &vector) {
    glUniform3fv(this->get_uniform_location(uniform), 1, &vector[0]);
}

void ShaderProgram::set_uniform_vec4f(ShaderUniform uniform, const glm::vec4 &vector) {
    glUniform4fv(this->get_uniform_location(uniform), 1, &vector[0]);
}

void ShaderProgram::set_uniform_mat4f(ShaderUniform uniform, const glm::mat4 &matrix) {
    glUniformMatrix4fv(this->get_uniform_location(uniform), 1, GL_FALSE, &matrix[0][0]);
}

void ShaderProgram::set_uniform_1i(ShaderUniform uniform, int value) {
    glUniform1i(this->get_uniform_location(uniform), value);
}

void ShaderProgram::set_uniform_1f(ShaderUniform uniform, float value) {
    glUniform1f(this->get_uniform_location(uniform), value);
}

void ShaderProgram::set_camera(Camera *new_cam) {
    if (this->camera != nullptr) {
        this->camera->unsubscribe(this);
    }
    this->camera = new_cam;
    this->camera->subscribe(this);

    this->use();
    this->set_uniform_mat4f(ShaderUniform::PROJECTION_MATRIX, this->camera->get_projection_matrix());
    this->set_uniform_mat4f(ShaderUniform::VIEW_MATRIX, this->camera->get_view_matrix());
    ShaderProgram::reset();
}

void ShaderProgram::set_light(Light *new_light) {
    if (this->light != nullptr) {
        this->light->unsubscribe(this);
    }
    this->light = new_light;
    this->light->subscribe(this);

    this->use();
    this->set_uniform_vec3f(ShaderUniform::LIGHT_WORLD_POSITION, this->light->get_position());
    this->set_uniform_vec3f(ShaderUniform::LIGHT_COLOR, this->light->get_color());
    this->set_uniform_1f(ShaderUniform::LIGHT_AMBIENT_STRENGTH, this->light->get_ambient_strength());
    this->set_uniform_1f(ShaderUniform::LIGHT_DIFFUSE_STRENGTH, this->light->get_diffuse_strength());
    this->set_uniform_1f(ShaderUniform::LIGHT_SPECULAR_STRENGTH, this->light->get_specular_strength());

    // should be handled by camera, but currently some shaders don't implement this uniform
    this->set_uniform_vec3f(ShaderUniform::CAMERA_WORLD_POSITION, this->camera->get_position());

    ShaderProgram::reset();
}

void ShaderProgram::update(int event) {
    this->use();

    switch (event) {
        case (int) CameraEvent::PROJECTION:
            this->set_uniform_mat4f(ShaderUniform::PROJECTION_MATRIX, this->camera->get_projection_matrix());
            return;
        case (int) CameraEvent::VIEW:
            this->set_uniform_mat4f(ShaderUniform::VIEW_MATRIX, this->camera->get_view_matrix());
            if (this->light != nullptr) {
                this->set_uniform_vec3f(ShaderUniform::CAMERA_WORLD_POSITION, this->camera->get_position());
            }
            return;
        case (int) LightEvent::POSITION:
            this->set_uniform_vec3f(ShaderUniform::LIGHT_WORLD_POSITION, this->light->get_position());
            return;
        case (int) LightEvent::COLOR:
            this->set_uniform_vec3f(ShaderUniform::LIGHT_COLOR, this->light->get_color());
            return;
        case (int) LightEvent::STRENGTH:
            this->set_uniform_1f(ShaderUniform::LIGHT_AMBIENT_STRENGTH, this->light->get_ambient_strength());
            this->set_uniform_1f(ShaderUniform::LIGHT_DIFFUSE_STRENGTH, this->light->get_diffuse_strength());
            this->set_uniform_1f(ShaderUniform::LIGHT_SPECULAR_STRENGTH, this->light->get_specular_strength());
            return;
        default:
            throw std::runtime_error("Unknown event type");
    }
}
