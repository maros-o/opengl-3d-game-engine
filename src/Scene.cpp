#include "Scene.h"


Scene::Scene(std::string name, const std::vector<RenderObject *> &render_objects) : name(std::move(name)) {
    this->renderer = new Renderer(render_objects);
}

Scene::Scene(std::string name, const std::vector<RenderObject *> &objects, std::vector<Transform *> transforms) : name(
        std::move(name)), transforms(std::move(transforms)) {
    this->renderer = new Renderer(objects);
}

Scene::~Scene() {
    delete this->renderer;
}

void Scene::play() {
    std::cout << "Playing scene: " << this->name << std::endl;

    InputManager &input_manager = InputManager::get_instance();
    OpenGLContext &context = OpenGLContext::get_instance();

    this->on_create();

    while (!context.should_close()) {
        InputManager::poll_events();
        input_manager.update();

        for (auto &transform: this->transforms) {
            transform->update_local_model_matrix();
        }

        for (auto &on_update: this->on_updates) {
            on_update();
        }

        Renderer::clear();

        if (this->sky_box != nullptr) {
            this->renderer->render_object(sky_box);
            Renderer::clear_depth();
        }

        this->renderer->render_all_objects();

        context.swap_buffers();
    }
}

void Scene::set_on_create(std::function<void()> new_on_create) {
    this->on_create = std::move(new_on_create);
}

void Scene::add_on_update(std::function<void()> new_on_update) {
    this->on_updates.push_back(std::move(new_on_update));
}

void Scene::set_sky_box(RenderObject *new_sky_box) {
    this->sky_box = new_sky_box;
}

void Scene::add_render_object(RenderObject *render_object) {
    this->renderer->add_object(render_object);
}

void Scene::add_transform(Transform *transform) {
    this->transforms.push_back(transform);
}

void Scene::remove_object_by_id(unsigned int id) {
    this->renderer->remove_object_by_id(id);
}
