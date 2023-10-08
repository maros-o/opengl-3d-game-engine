#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "OpenGLContext.h"

class OrthographicCamera {
public:
    explicit OrthographicCamera(OpenGLContext *context);

    [[nodiscard]] glm::mat4 get_view_projection_matrix() const;

    void translate(glm::vec3 translation);

    void rotate(float angle, glm::vec3 axis);


private:
    glm::mat4 projection_matrix{glm::mat4(1.0f)};
    glm::mat4 view_matrix{glm::mat4(1.0f)};
};
