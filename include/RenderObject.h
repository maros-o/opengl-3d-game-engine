#pragma once

#include <functional>

#include "Model.h"
#include "./Transform/Transform.h"
#include "./Transform/TransformLeaf.h"

class RenderObject {
public:
    explicit RenderObject(Model *model, ShaderProgram *shader, Material material) : model(model), material(material),
                                                                                    shader(shader) {
        this->id = id_counter++;
    };

    Model *get_model();

    Transform *get_transform();

    Material &get_material();

    ShaderProgram *get_shader();

    [[nodiscard]] unsigned int get_id() const;

    void set_transform(Transform *new_transform);

private:
    static unsigned int id_counter;

    unsigned int id;
    Model *model;
    Material material;
    ShaderProgram *shader;
    Transform *transform = new TransformLeaf();

};

