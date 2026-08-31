#pragma once
#include "glad/gl.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/component_wise.hpp>
#include "include/mesh.hh"
#include "include/matrices.hh"
#include <vector>
#include <iostream>
#include <string>

class GPUMesh
{
public:
    glm::vec3 min_bounds;
    glm::vec3 max_bounds;
    glm::vec3 center;
    glm::vec3 extent;
    float span;
    glm::mat4 to_unit_extent; // normalization model matrix
    glm::vec3 unit_center;
    glm::vec3 unit_extent;
    float unit_span;

private:
    std::vector<float> points = {};
    std::vector<unsigned int> indices = {};

    GLuint vbo;
    GLuint ebo;
    GLuint vao;
    bool initialized = false;

public:
    GPUMesh (std::string filename){ load (filename); }

    ~GPUMesh () { clean (); }

    void load (std::string filename)
    {
        fcg::Mesh mesh (filename);
        mesh.pack4gpu (points, indices);
        send_arrays_2a3f ();

        min_bounds = mesh.min_bounds;
        max_bounds = mesh.max_bounds;
        center = (min_bounds + max_bounds) * 0.5f;
        span = glm::distance (max_bounds, min_bounds);
        extent = max_bounds - min_bounds;

        std::cout <<"MESH: "<< filename << "\n";
        std::cout <<"(original) center, extent, span:" << "\n";
        std::cout << center.x <<" "<< center.y <<" "<< center.z << "\n";
        std::cout << extent.x <<" "<< extent.y <<" "<< extent.z << "\n";
        std::cout << span << "\n";

        to_unit_extent =
            fcg::scaling (1.0 / glm::compMax (extent)) *
            fcg::translation (-center);

        unit_center = {0.0, 0.0, 0.0};
        unit_span = glm::distance (extent, {0.0, 0.0, 0.0});
        unit_extent = extent / glm::compMax (extent);

        std::cout <<"(unit normalized) center, extent, span:" << "\n";
        std::cout << unit_center.x <<" "<< unit_center.y <<" "<< unit_center.z << "\n";
        std::cout << unit_extent.x <<" "<< unit_extent.y <<" "<< unit_extent.z << "\n";
        std::cout << unit_span << "\n\n";

        initialized = true;
    }

    void clean ()
    {
        if (initialized) {
            glDeleteVertexArrays (1, &vao);
            glDeleteBuffers (1, &vbo);
        }
    }

    void draw ()
    {
        glBindVertexArray (vao);
        glDrawElements(GL_TRIANGLES, indices.size (), GL_UNSIGNED_INT, 0);
    }

protected:
    // send to the gpu the mesh arrays:
    // - the mesh vertices, 2 attributes, 3 floats each
    // - the mesh indices
    void send_arrays_2a3f ()
    {
        // we want just one buffer, and we retrieve the name OpenGL assigns to it.
        glGenBuffers (1, &vbo);
        // bind it as the current VBO
        glBindBuffer (GL_ARRAY_BUFFER, vbo);
        // transfer data from CPU RAM to GPU RAM.
        glBufferData (GL_ARRAY_BUFFER,
                      points.size () * sizeof (float),
                      points.data (),
                      GL_STATIC_DRAW);

        // we want just one buffer container, and we retrieve the name OpenGL assigns to it.
        glGenVertexArrays (1, &vao);
        // bind it as the current vao.
        glBindVertexArray (vao);

        // Attribute 0: position (x, y, z)
        glVertexAttribPointer (0,
                               3,
                               GL_FLOAT,
                               GL_FALSE,
                               6 * sizeof(float),
                               (void*)0);
        glEnableVertexAttribArray (0);

        // Attribute 1: 3 generic floats (u, v, w)
        glVertexAttribPointer (1,
                               3,
                               GL_FLOAT,
                               GL_FALSE,
                               6 * sizeof(float),
                               (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray (1);

        glGenBuffers(1, &ebo); 
        // MUST be bound after the VAO's binding!
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     indices.size () * sizeof (unsigned int),
                     indices.data (),
                     GL_STATIC_DRAW);
    }
};