#pragma once

#include <vector>
#include <unordered_map>

#include "RenderObject.h"

class Renderer {
public:
    Renderer() = default;

    void add_object(RenderObject *object);

    void render_objects();

    static void clear();

private:
    std::unordered_map<std::string, std::vector<RenderObject *>> objects;
    std::unordered_map<std::string, Model *> models;

    GLsizei draw_count = 0;

    ShaderProgram *shader = nullptr;
};