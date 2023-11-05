#pragma once

#include <vector>

#include "Renderer.h"
#include "RenderObject.h"
#include "InputManager.h"
#include "Transform/Transform.h"

class Scene {
public:
    Scene(std::string name, const std::vector<RenderObject *> &render_objects);

    Scene(std::string name, const std::vector<RenderObject *> &render_objects,
          std::vector<Transform *> objectless_transforms);

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

    std::vector<Transform *> transforms;

    std::function<void()> on_create = []() {};
    std::function<void()> on_update = []() {};
};