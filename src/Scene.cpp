#include "Scene.h"


Scene::Scene(std::string name) : name(std::move(name)) {
    this->renderer = new Renderer();

}

Scene::Scene(std::string name, const std::vector<RenderObject *> &objects) : name(std::move(name)) {
    this->renderer = new Renderer(objects);
}

Scene::~Scene() {
    delete this->renderer;
}

void Scene::add_object(RenderObject *object) {
    this->renderer->add_object(object);
}

void Scene::play() {
    std::cout << "Playing scene: " << this->name << std::endl;
    Engine::run(this->renderer);
}



