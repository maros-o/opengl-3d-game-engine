#pragma once

#include <vector>
#include <unordered_map>

#include "RenderObject.h"
#include "OrthographicCamera.h"

class Renderer {
public:
    Renderer() = default;

    void add_object(RenderObject *object);

    void render() const;

    void render_all_objects(OrthographicCamera *camera);

    static void clear();

private:
    std::unordered_map<std::string, std::vector<RenderObject *>> objects;
    std::unordered_map<std::string, Model *> models;

    GLsizei draw_elements_count = 0;
    ShaderProgram *shader = nullptr;
};