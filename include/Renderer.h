#pragma once

#include <vector>
#include <unordered_map>

#include "RenderObject.h"

class Renderer {
public:
    Renderer() = default;

    void add_object(RenderObject *object);

    void render(RenderObject *object) const;

    void render_all_objects();

    void render_all_objects(float rotation);

    static void clear();

private:
    std::unordered_map<std::string, std::vector<RenderObject *>> objects;
    std::unordered_map<std::string, Model *> models;

    GLsizei draw_elements_count = 0;
    ShaderProgram *shader = nullptr;
};