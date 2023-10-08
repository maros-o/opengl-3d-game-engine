#pragma once

#include "OpenGLContext.h"

std::vector<GLuint> indices_triangle = {
        0, 1, 2
};

std::vector<GLuint> indices_square = {
        0, 1, 2,
        2, 3, 0
};

std::vector<GLuint> indices_cube = {
        // front
        0, 1, 2,
        2, 3, 0,

        // top
        0, 1, 5,
        5, 4, 0,

        // right
        1, 2, 6,
        6, 5, 1,

        // bottom
        2, 3, 7,
        7, 6, 2,

        // left
        3, 0, 4,
        4, 7, 3,

        // back
        4, 5, 6,
        6, 7, 4
};