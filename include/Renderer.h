#pragma once

#include <vector>
#include <unordered_map>

#include "RenderObject.h"
#include "OrthoCamera.h"

class Renderer {
public:
    Renderer() = default;

    void add_object(RenderObject *object);

    void render() const;

    void render_objects(OrthoCamera *camera);

    void update_model_matrices();

    static void clear();

private:
    std::unordered_map<std::string, std::vector<RenderObject *>> objects;
    std::unordered_map<std::string, Model *> models;

    GLsizei draw_elements_count = 0;
    ShaderProgram *shader = nullptr;
};