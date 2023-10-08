#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "OpenGLContext.h"

class OrthoCamera {
public:
    explicit OrthoCamera(int width, int height);

    [[nodiscard]] glm::mat4 get_view_projection_matrix() const;

private:
    glm::mat4 projection_matrix{glm::mat4(1.0f)};
    glm::mat4 view_matrix{glm::mat4(1.0f)};
};
