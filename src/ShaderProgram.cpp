#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

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

ShaderProgram::ShaderProgram(const char *shader_file_path) {
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

GLint ShaderProgram::get_uniform_location(const char *name) {
    if (this->uniform_locations.find(name) == this->uniform_locations.end()) {
        GLint location = glGetUniformLocation(this->id, name);
        if (location == -1) {
            std::cerr << "ERROR::SHADER::UNIFORM::" << name << "::NOT_FOUND" << std::endl;
            exit(1);
        }
        this->uniform_locations[name] = location;
    }
    return this->uniform_locations[name];
}

void ShaderProgram::set_uniform_vec3f(const char *name, const glm::vec3 &vector) {
    glUniform3fv(this->get_uniform_location(name), 1, &vector[0]);
}

void ShaderProgram::set_uniform_vec4f(const char *name, const glm::vec4 &vector) {
    glUniform4fv(this->get_uniform_location(name), 1, &vector[0]);
}

void ShaderProgram::set_uniform_mat4f(const char *name, const glm::mat4 &matrix) {
    GLint location = this->get_uniform_location(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void ShaderProgram::set_uniform_1i(const char *name, int value) {
    GLint location = this->get_uniform_location(name);
    glUniform1i(location, value);
}
