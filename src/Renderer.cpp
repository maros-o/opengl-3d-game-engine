#include "Renderer.h"
#include "RenderObject.h"


void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(RenderObject *object) const {
    this->shader->set_uniform_mat4("model_matrix", object->get_model_matrix());
    glDrawElements(GL_TRIANGLES, this->draw_elements_count, GL_UNSIGNED_INT, nullptr);
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

void Renderer::render_all_objects() {
    for (auto &object: this->objects) {
        auto model = this->models[object.first];

        model->get_shader()->use();
        model->get_vao()->bind();
        this->draw_elements_count = model->get_vao()->get_ebo()->get_count();
        this->shader = model->get_shader();

        for (auto &render_object: object.second) {
            Renderer::render(render_object);
        }
    }
}

void Renderer::render_all_objects(float rotation) {
    for (auto &object: this->objects) {
        auto model = this->models[object.first];

        model->get_shader()->use();
        model->get_vao()->bind();
        this->draw_elements_count = model->get_vao()->get_ebo()->get_count();
        this->shader = model->get_shader();

        for (auto &render_object: object.second) {
            render_object->rotate(rotation, glm::vec3(1.0f, 1.0f, .5f));
            Renderer::render(render_object);
        }
    }
}
