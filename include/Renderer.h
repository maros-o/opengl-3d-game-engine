#pragma once

#include <vector>
#include <unordered_map>

#include "RenderObject.h"


class Renderer {
public:
    Renderer() = default;

    explicit Renderer(const std::vector<RenderObject *> &objects);

    void add_object(RenderObject *object);

    void remove_object(RenderObject *object);

    void remove_object_by_id(unsigned int id);

    void render_all_objects();

    void render_object(RenderObject *object);

    void render() const;

    static void clear();

    static void clear_depth();

    static void clear_color();

private:
    std::unordered_map<std::string, std::vector<RenderObject *>> objects;
    std::unordered_map<std::string, Model *> models;

    GLsizei current_draw_count = 0;
    bool current_vao_has_ebo = false;

    void render_single_object(RenderObject *object) const;
};