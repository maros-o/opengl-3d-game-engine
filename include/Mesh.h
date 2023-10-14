#pragma once

#include <vector>

class Mesh {
public:
    Mesh() = default;

    Mesh(std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<float> normals,
         std::vector<float> texture_coords) : vertices(std::move(vertices)), indices(std::move(indices)),
                                              normals(std::move(normals)), texture_coords(std::move(texture_coords)) {};

    inline const std::vector<float> &get_vertices() const {
        return this->vertices;
    }

    inline const std::vector<unsigned int> &get_indices() const {
        return this->indices;
    }

    inline const std::vector<float> &get_normals() const {
        return this->normals;
    }

    inline const std::vector<float> &get_texture_coords() const {
        return this->texture_coords;
    }

private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> normals;
    std::vector<float> texture_coords;
};