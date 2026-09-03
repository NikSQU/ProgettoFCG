#pragma once
#include "glad/gl.h"
#include <glm/glm.hpp>

#include "camera.hh"
#include "lights.hh"
#include "gpumesh.hh"
#include <string>

// Forward declaration
namespace fcg { class Shaders; }

class Scene {
public:
    Scene(std::string meshfile, fcg::Shaders& shaders);
    float velocita_rotore = 0.5f;
    float velocita_salvata = 0.5f;
    float angolo_rotore = 0.0f;

    // TAPPA 12: Variabili Giorno/Notte
    float tempo_giorno = 0.0f; 
    float velocita_giorno = 0.005f;
    glm::vec3 colore_cielo = glm::vec3(0.4f, 0.6f, 0.9f);
    glm::vec3 posizione_sole;

    Camera camera;
    Lights lights;
    
    GPUMesh mesh;
    glm::mat4 mesh_mm;

    GLint model_loc;
    GLint vp_loc; 
    GLint tr_inv_model_loc;

    void locations(fcg::Shaders& shaders);
    void update_all();
    void draw();
    
};