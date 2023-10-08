#pragma once

#include "OpenGLContext.h"

std::vector<GLfloat> vertices_triangle_rgb = {
        0.0f, 0.2f, 0.0f,
        1.0f, 0.0f, 0.0f,

        0.2f, -0.2f, 0.0f,
        0.0f, 1.0f, 0.0f,

        -0.2f, -0.2f, 0.0f,
        0.0f, 0.0f, 1.0f
};

std::vector<GLfloat> vertices_square_rgb = {
        -0.5, 0.5, 0.0,
        1.0, 1.0, 0.0,

        0.5, 0.5, 0.0,
        0.0, 1.0, 1.0,

        0.5, -0.5, 0.0,
        1.0, 0.0, 1.0,

        -0.5, -0.5, 0.0,
        1.0, 1.0, 1.0
};

std::vector<GLfloat> vertices_square_texture = {
        -0.5, 0.5, 0.0,
        0.0, 1.0,

        0.5, 0.5, 0.0,
        1.0, 1.0,

        0.5, -0.5, 0.0,
        1.0, 0.0,

        -0.5, -0.5, 0.0,
        0.0, 0.0
};

std::vector<GLfloat> vertices_cube = {
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

std::vector<GLfloat> vertices_cube_rgb = {
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

std::vector<GLfloat> vertices_cube_texture = {
        // front
        -0.5, 0.5, 0.5,     // 0 left top front
        0.0, 1.0,

        0.5, 0.5, 0.5,      // 1 right top front
        1.0, 1.0,

        0.5, -0.5, 0.5,     // 2 right bottom front
        1.0, 0.0,

        -0.5, -0.5, 0.5,   // 3 left bottom front
        0.0, 0.0,

        // back
        -0.5, 0.5, -0.5,  // 4 left top back
        0.0, 1.0,

        0.5, 0.5, -0.5,   // 5 right top back
        1.0, 1.0,

        0.5, -0.5, -0.5,  // 6 right bottom back
        1.0, 0.0,

        -0.5, -0.5, -0.5, // 7 left bottom back
        0.0, 0.0,
};