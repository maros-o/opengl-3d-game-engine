#pragma once

#include "OpenGLContext.h"

GLfloat vertices_triangle_rgb[] = {
        0.0f, 0.2f, 0.0f,
        1.0f, 0.0f, 0.0f,

        0.2f, -0.2f, 0.0f,
        0.0f, 1.0f, 0.0f,

        -0.2f, -0.2f, 0.0f,
        0.0f, 0.0f, 1.0f
};

GLuint indices_triangle[] = {
        0, 1, 2
};


GLfloat vertices_square_rgb[] = {
        -0.5, 0.5, 0.0,
        1.0, 1.0, 0.0,

        0.5, 0.5, 0.0,
        0.0, 1.0, 1.0,

        0.5, -0.5, 0.0,
        1.0, 0.0, 1.0,

        -0.5, -0.5, 0.0,
        1.0, 1.0, 1.0
};

GLuint indices_square[] = {
        0, 1, 2,
        2, 3, 0
};

GLfloat vertices_cube[] = {
        // front
        -0.5, 0.5, 0.5,     // 0 left top front
        0.5, 0.5, 0.5,      // 1 right top front
        0.5, -0.5, 0.5,     // 2 right bottom front
        -0.5, -0.5, 0.5,   // 3 left bottom front

        // back
        -0.5, 0.5, -0.5,  // 4 left top back
        0.5, 0.5, -0.5,   // 5 right top back
        0.5, -0.5, -0.5,  // 6 right bottom back
        -0.5, -0.5, -0.5, // 7 left bottom back
};

GLfloat vertices_cube_rgb[] = {
        // front
        -0.5, 0.5, 0.5,     // 0 left top front
        1.0, 0.0, 0.0,

        0.5, 0.5, 0.5,      // 1 right top front
        0.0, 1.0, 0.0,

        0.5, -0.5, 0.5,     // 2 right bottom front
        0.0, 0.0, 1.0,

        -0.5, -0.5, 0.5,   // 3 left bottom front
        1.0, 1.0, 0.0,

        // back
        -0.5, 0.5, -0.5,  // 4 left top back
        1.0, 0.0, 1.0,

        0.5, 0.5, -0.5,   // 5 right top back
        0.0, 1.0, 0.0,

        0.5, -0.5, -0.5,  // 6 right bottom back
        1.0, 1.0, 1.0,

        -0.5, -0.5, -0.5, // 7 left bottom back
        0.0, 0.0, 0.0,
};

GLuint indices_cube[] = {
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