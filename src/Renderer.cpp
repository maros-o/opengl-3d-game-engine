#include "Renderer.h"
#include "RenderObject.h"
#include "Texture.h"

Renderer::Renderer(const std::vector<RenderObject *> &objects) {
    for (auto &object: objects) {
        this->add_object(object);
    }
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

void Renderer::render() const {
    if (this->current_vao_has_ebo) {
        glDrawElements(GL_TRIANGLES, this->current_draw_count, GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, this->current_draw_count);
    }
}

void Renderer::clear_depth() {
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::clear_color() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render_all_objects() {
    for (auto &model_group: this->objects) {
        auto model = this->models[model_group.first];
        model->get_vao()->bind();

        this->current_vao_has_ebo = model->get_vao()->has_ebo();
        this->current_draw_count = this->current_vao_has_ebo ? model->get_vao()->get_ebo().value().get_indices_count()
                                                             : model->get_vao()->get_vbo().get_vertex_count();

        for (auto &render_object: model_group.second) {
            this->render_single_object(render_object);
        }
    }
}

void Renderer::render_object(RenderObject *render_object) {
    auto model = render_object->get_model();
    model->get_vao()->bind();

    this->current_vao_has_ebo = model->get_vao()->has_ebo();
    this->current_draw_count = this->current_vao_has_ebo ? model->get_vao()->get_ebo().value().get_indices_count()
                                                         : model->get_vao()->get_vbo().get_vertex_count();

    this->render_single_object(render_object);
}

void Renderer::render_single_object(RenderObject *render_object) const {
    auto shader = render_object->get_shader();
    shader->use();

    shader->set_uniform(ShaderUniform::MODEL_MATRIX,
                        render_object->get_transform()->get_model_matrix());

    auto material = render_object->get_material();
    shader->set_uniform(ShaderUniform::OBJECT_COLOR, material.get_object_color());
    shader->set_uniform(ShaderUniform::OBJECT_AMBIENT, material.get_ambient_strength());
    shader->set_uniform(ShaderUniform::OBJECT_DIFFUSE, material.get_diffuse_strength());
    shader->set_uniform(ShaderUniform::OBJECT_SPECULAR, material.get_specular_strength());
    shader->set_uniform(ShaderUniform::OBJECT_SHININESS, material.get_shininess());

    if (material.get_texture() != nullptr) {
        shader->set_uniform(ShaderUniform::TEXTURE_SAMPLER, 0);
        shader->set_uniform(ShaderUniform::TEXTURE_REPEAT, material.get_texture()->get_repeat());
        material.get_texture()->bind();
    }

    this->render();
}
