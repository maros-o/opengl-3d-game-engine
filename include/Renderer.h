#pragma once

#include <vector>
#include <unordered_map>

#include "RenderObject.h"


class Renderer {
public:
    Renderer() = default;

    explicit Renderer(const std::vector<RenderObject *> &objects);

    void add_object(RenderObject *object);

    void render_objects();

    void render() const;

    static void clear();

private:
    std::unordered_map<std::string, std::vector<RenderObject *>> objects;
    std::unordered_map<std::string, Model *> models;

    GLsizei current_draw_count = 0;
    bool current_vao_has_ebo = false;
};