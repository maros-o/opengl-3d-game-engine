#pragma once

#include <assimp/Importer.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/vec4.hpp>

#include "Model.h"


class ModelLoader {
public:
    static std::vector<Model *> load_models(const std::string &file_name) {
        Assimp::Importer importer;
        const auto *ai_scene = importer.ReadFile(file_name, aiProcess_Triangulate
                                                            | aiProcess_OptimizeMeshes
                                                            | aiProcess_JoinIdenticalVertices
                                                            | aiProcess_CalcTangentSpace);

        if (ai_scene == nullptr) {
            fprintf(stderr, "ERROR::ASSIMP_LOAD::AI_SCENE:nullptr\n");
            exit(EXIT_FAILURE);
        }
        printf("ai_scene->mNumMeshes = %d\n", ai_scene->mNumMeshes);
        printf("ai_scene->mNumMaterials = %d\n", ai_scene->mNumMaterials);

        std::vector<Model *> models;

        for (unsigned int i = 0; i < ai_scene->mNumMeshes; i++) {
            std::vector<GLfloat> vbo_data = {};

            aiMesh *mesh = ai_scene->mMeshes[i];
            bool has_positions = mesh->HasPositions();
            bool has_normals = mesh->HasNormals();
            bool has_texture_coords = mesh->HasTextureCoords(0);
            bool has_tangents = mesh->HasTangentsAndBitangents();
            bool has_faces = mesh->HasFaces();

            GLsizei vertex_size = 0;
            if (has_positions) vertex_size += 3;
            if (has_normals) vertex_size += 3;
            if (has_texture_coords) vertex_size += 2;
            if (has_tangents) vertex_size += 3;
            vertex_size *= sizeof(GLfloat);

            std::cout << "mesh: " << mesh->mName.C_Str() << std::endl;
            std::cout << "vertex_size = " << vertex_size << std::endl;
            std::cout << "has_positions = " << has_positions << std::endl;
            std::cout << "has_normals = " << has_normals << std::endl;
            std::cout << "has_texture_coords = " << has_texture_coords << std::endl;
            std::cout << "has_tangents = " << has_tangents << std::endl;
            std::cout << "has_faces = " << has_faces << std::endl << std::endl;

            for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
                if (has_positions) {
                    vbo_data.emplace_back(mesh->mVertices[j].x);
                    vbo_data.emplace_back(mesh->mVertices[j].y);
                    vbo_data.emplace_back(mesh->mVertices[j].z);
                }
                if (has_normals) {
                    vbo_data.emplace_back(mesh->mNormals[j].x);
                    vbo_data.emplace_back(mesh->mNormals[j].y);
                    vbo_data.emplace_back(mesh->mNormals[j].z);
                }
                if (has_texture_coords) {
                    vbo_data.emplace_back(mesh->mTextureCoords[0][j].x);
                    vbo_data.emplace_back(mesh->mTextureCoords[0][j].y);
                }
                if (has_tangents) {
                    vbo_data.emplace_back(mesh->mTangents[j].x);
                    vbo_data.emplace_back(mesh->mTangents[j].y);
                    vbo_data.emplace_back(mesh->mTangents[j].z);
                }
            }

            std::vector<GLuint> ebo_data = {};
            for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
                ebo_data.emplace_back(mesh->mFaces[j].mIndices[0]);
                ebo_data.emplace_back(mesh->mFaces[j].mIndices[1]);
                ebo_data.emplace_back(mesh->mFaces[j].mIndices[2]);
            }

            auto vao = has_faces ? new VAO{VBO{vbo_data, vertex_size}, EBO{ebo_data}} : new VAO{
                    VBO{vbo_data, vertex_size}};
            GLuint layout = 0;
            size_t stride = 0;

            if (has_positions) {
                vao->link_attributes(layout++, 3, GL_FLOAT, vertex_size, nullptr);
                stride += 3 * sizeof(GLfloat);
            }
            if (has_normals) {
                vao->link_attributes(layout++, 3, GL_FLOAT, vertex_size, (void *) (stride));
                stride += 3 * sizeof(GLfloat);
            }
            if (has_texture_coords) {
                vao->link_attributes(layout++, 2, GL_FLOAT, vertex_size, (void *) (stride));
                stride += 2 * sizeof(GLfloat);
            }
            if (has_tangents) {
                vao->link_attributes(layout, 3, GL_FLOAT, vertex_size, (void *) (stride));
            }

            models.push_back(new Model{mesh->mName.C_Str(), vao});
        }
        return models;
    }
};