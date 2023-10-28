#include "Renderer.h"
#include "RenderObject/RenderObject.h"
#include "Texture.h"

Renderer::Renderer(const std::vector<RenderObject *> &objects) {
    for (auto &object: objects) {
        this->add_object(object);
    }
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::add_object(RenderObject *object) {
    auto model_name = object->get_model()->get_name();

    if (this->objects.find(model_name) == this->objects.end()) {
        this->objects[model_name] = std::vector<RenderObject *>({object});
        this->models[model_name] = object->get_model();
    } else {
        this->objects[model_name].push_back(object);
    }
}

void Renderer::render_objects() {
    for (auto &model_group: this->objects) {
        auto model = this->models[model_group.first];

        model->get_shader()->use();
        model->get_vao()->bind();

        this->current_vao_has_ebo = model->get_vao()->has_ebo();
        this->current_shader = model->get_shader();
        this->current_draw_count = this->current_vao_has_ebo ? model->get_vao()->get_ebo()->get_indices_count()
                                                             : model->get_vao()->get_vbo()->get_vertex_count();

        Material *model_material = model->get_material();
        this->current_shader->set_uniform(ShaderUniform::OBJECT_COLOR, model_material->get_object_color());
        this->current_shader->set_uniform(ShaderUniform::OBJECT_AMBIENT, model_material->get_ambient_strength());
        this->current_shader->set_uniform(ShaderUniform::OBJECT_DIFFUSE, model_material->get_diffuse_strength());
        this->current_shader->set_uniform(ShaderUniform::OBJECT_SPECULAR, model_material->get_specular_strength());
        this->current_shader->set_uniform(ShaderUniform::OBJECT_SHININESS, model_material->get_shininess());

        for (auto &render_object: model_group.second) {
            this->current_shader->set_uniform(ShaderUniform::MODEL_MATRIX, render_object->get_model_matrix());

            this->render();
        }
    }
}

void Renderer::render() const {
    if (this->current_vao_has_ebo) {
        glDrawElements(GL_TRIANGLES, this->current_draw_count, GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, this->current_draw_count);
    }
}
