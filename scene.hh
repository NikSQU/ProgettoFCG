#pragma once
#include "glad/gl.h"
#include <glm/glm.hpp>
#include "input.hh"
#include "camera.hh" 
#include "mesh.hh"
#include "lights.hh"

// Forward declaration
namespace fcg { class Shaders; }

class Scene {
public:
    float velocita_rotore = 0.5f;
    float velocita_salvata = 0.5f;
    float angolo_rotore = 0.0f;

    Camera camera;
    Lights lights;
    
    Mesh mesh;
    glm::mat4 mesh_mm;

    GLint model_loc;
    GLint tr_inv_model_loc;

    void locations(fcg::Shaders& shaders);
    void update_all();
    void draw();
};