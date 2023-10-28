#pragma once

#include <vector>

#include "Renderer.h"
#include "RenderObject/RenderObject.h"
#include "InputManager.h"

class Scene {
public:
    explicit Scene(std::string name, const std::vector<RenderObject *> &objects);

    ~Scene();

    void add_object(RenderObject *object);

    void play();

    void stop();

    void set_on_create(std::function<void()> new_on_create);

    void set_on_update(std::function<void()> new_on_update);


private:
    std::string name;
    Renderer *renderer;

    bool is_playing = false;

    std::vector<RenderObject *> objects;

    std::function<void()> on_create = []() {};
    std::function<void()> on_update = []() {};
};