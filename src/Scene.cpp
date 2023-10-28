#include "Scene.h"


Scene::Scene(std::string name, const std::vector<RenderObject *> &objects, std::vector<Transform *> transforms) : name(
        std::move(name)) {
    this->renderer = new Renderer(objects);
    this->transforms = std::move(transforms);
}

Scene::~Scene() {
    delete this->renderer;
}

void Scene::add_object(RenderObject *object) {
    this->renderer->add_object(object);
}

void Scene::play() {
    std::cout << "Playing scene: " << this->name << std::endl;
    this->is_playing = true;

    InputManager &input_manager = InputManager::get_instance();
    OpenGLContext &context = OpenGLContext::get_instance();

    this->on_create();

    while (!context.should_close() && this->is_playing) {
        InputManager::poll_events();
        input_manager.update();

        Renderer::clear();
        for (auto &transform: this->transforms) {
            transform->update_local_model_matrix();
        }
        this->renderer->render_objects();

        this->on_update();

        context.swap_buffers();
    }
}

void Scene::set_on_create(std::function<void()> new_on_create) {
    this->on_create = std::move(new_on_create);
}

void Scene::set_on_update(std::function<void()> new_on_update) {
    this->on_update = std::move(new_on_update);
}

void Scene::stop() {
    this->is_playing = false;
}
