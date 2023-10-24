#pragma once

#include <vector>

#include "Renderer.h"
#include "RenderObject/RenderObject.h"
#include "Engine.h"

class Scene {
public:
    explicit Scene(std::string name);

    explicit Scene(std::string name, const std::vector<RenderObject *> &objects);

    ~Scene();

    void add_object(RenderObject *object);

    void play();

private:
    std::string name;
    Renderer *renderer;

    std::vector<RenderObject *> objects;
};