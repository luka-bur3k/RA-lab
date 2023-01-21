#pragma once

#include <GL/glut.h>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>

#include "mesh.hh"
#include "racani_utils.hh"

int loadOBJ(std::string&& pathToFile, std::vector<std::unique_ptr<Mesh>>& meshes)
{

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(pathToFile, aiProcess_Triangulate | aiProcess_GenNormals);

    // Check for errors
    if ((!scene) || (scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE) || (!scene->mRootNode))
    {
        std::cerr << "Error loading" + pathToFile + ": " << std::string(importer.GetErrorString()) << std::endl;
        // Return fail
        return -1;
    }

    // Iterate over the meshes
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        // Get the mesh
        aiMesh *mesh = scene->mMeshes[i];

        // Create vectors for the vertices and indices
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        // Iterate over the vertices of the mesh
        for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
        {
            // Create a vertex to store the mesh's vertices temporarily
            Vertex tempVertex;

            // Set the positions
            tempVertex.position.x = mesh->mVertices[j].x;
            tempVertex.position.y = mesh->mVertices[j].y;
            tempVertex.position.z = mesh->mVertices[j].z;

            // Set the normals
            tempVertex.normal.x = mesh->mNormals[j].x;
            tempVertex.normal.y = mesh->mNormals[j].y;
            tempVertex.normal.z = mesh->mNormals[j].z;

            // Add the vertex to the vertices vector
            vertices.push_back(tempVertex);
        }

        // Iterate over the faces of the mesh
        for (unsigned int j = 0; j < mesh->mNumFaces; ++j)
        {
            // Get the face
            aiFace face = mesh->mFaces[j];
            // Add the indices of the face to the vector
            for (unsigned int k = 0; k < face.mNumIndices; ++k)
            {
                indices.push_back(face.mIndices[k]);
            }
        }

        // Create a new mesh and add it to the vector
        meshes.push_back(std::unique_ptr<Mesh>(new Mesh(std::move(vertices), std::move(indices))));
    }

    return 0;
}