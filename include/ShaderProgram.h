#pragma once

#include <GL/glew.h>
#include <iostream>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <unordered_map>

class ShaderProgram {
public:
    explicit ShaderProgram(const char *shader_file_path);

    ~ShaderProgram();

    void use() const;

    static void reset();

    void set_uniform_vec3(const char *name, const glm::vec3 &vector);

    void set_uniform_vec4(const char *name, const glm::vec4 &vector);

    void set_uniform_mat4(const char *name, const glm::mat4 &matrix);

    void print_active_uniforms() const;

private:
    GLuint id = 0;
    std::unordered_map<const char *, GLint> uniform_locations;

    void destroy() const;

    GLint get_uniform_location(const char *name);
};
