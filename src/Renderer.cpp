#include "Renderer.h"
#include "RenderObject.h"
#include "Texture.h"


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

        this->draw_count = model->get_vao()->has_ebo() ? model->get_vao()->get_ebo()->get_indices_count()
                                                       : model->get_vao()->get_vbo()->get_vertex_count();
        this->shader = model->get_shader();


        if (model->get_texture() != nullptr) {
            model->get_texture()->bind();
            this->shader->set_uniform_1i("uni_texture_sampler", 0);
        }

        for (auto &render_object: model_group.second) {
            this->shader->set_uniform_mat4f("uni_model_matrix", render_object->get_model_matrix());

            if (model->get_vao()->has_ebo()) {
                glDrawElements(GL_TRIANGLES, this->draw_count, GL_UNSIGNED_INT, nullptr);
            } else {
                glDrawArrays(GL_TRIANGLES, 0, this->draw_count);
            }
        }
    }
}
