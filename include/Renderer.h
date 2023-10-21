#pragma once

#include <vector>
#include <unordered_map>

#include "RenderObject/RenderObject.h"


class Renderer {
public:
    Renderer() = default;

    void add_object(RenderObject *object);

    void render_objects();

    void render() const;

    static void clear();

private:
    std::unordered_map<std::string, std::vector<RenderObject *>> objects;
    std::unordered_map<std::string, Model *> models;

    GLsizei current_draw_count = 0;
    ShaderProgram *current_shader = nullptr;
    bool current_vao_has_ebo = false;
};