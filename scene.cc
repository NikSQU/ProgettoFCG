#include "scene.hh"
#include "include/hotshaders.hh"
#include <glm/ext/matrix_transform.hpp>

Scene::Scene (std::string meshfile, fcg::Shaders& shaders) : camera (shaders), lights (shaders), mesh (meshfile) {
    locations (shaders);
    update_all ();
    mesh_mm = mesh.to_unit_extent;
}

void Scene::locations (fcg::Shaders& shaders)
    {
        camera.locations (shaders);
        lights.locations (shaders);

        model_loc = glGetUniformLocation (shaders.program, "model");
        vp_loc = glGetUniformLocation (shaders.program, "vp");
        tr_inv_model_loc = glGetUniformLocation (shaders.program, "tr_inv_model");
    }

void Scene::update_all ()
    {
        camera.view_projection ();
        lights.send_parameters ();
        lights.send_position_relative (camera.inv_v);
    }

void Scene::process_movement() 
{
    float speed = 0.05f;
    float fwd = 0.0f, rgt = 0.0f, up = 0.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) fwd += speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) fwd -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) rgt += speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) rgt -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space)) up += speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift)) up -= speed;

    if (fwd != 0.0f || rgt != 0.0f || up != 0.0f) {
        camera.move(fwd, rgt, up);
        lights.send_position_relative(camera.inv_v);
    }
}

void Scene::draw ()
    {
        //colore cielo
        glClearColor(0.4f, 0.6f, 0.9f, 1.0f);
        //Pulizia dei buffer dello schermo
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ===========================================================
        // TAPPA 01: Il Suolo del Mulino
        // ===========================================================
        
        //Colori suolo
        lights.material_diffuse = {0.2f, 0.6f, 0.2f};
        lights.material_ambient = {0.1f, 0.3f, 0.1f};
        lights.send_parameters();

        glm::mat4 s_suolo = fcg::scaling (6.0, 0.2, 6.0);
        glm::mat4 tr_suolo = fcg::translation (0.0, -1.0, 0.0);
        glm::mat4 suolo_mm = tr_suolo * s_suolo * mesh_mm;

        glUniformMatrix4fv(model_loc, 1, GL_FALSE, &suolo_mm[0][0]);
        glUniformMatrix4fv(vp_loc, 1, GL_FALSE, &camera.vp[0][0]);

        glm::mat3 tr_inv_suolo = glm::transpose (glm::inverse (glm::mat3 (suolo_mm)));
        glUniformMatrix3fv (tr_inv_model_loc, 1, GL_FALSE, &tr_inv_suolo[0][0]);

        mesh.draw ();

        // ===========================================================
        // TAPPA 02: La Torre del Mulino (Rimpicciolita e Cilindrica)
        // ===========================================================

        //Colori torre
        lights.material_diffuse = {0.35f, 0.35f, 0.35f}; 
        lights.material_ambient = {0.2f, 0.2f, 0.2f};
        lights.send_parameters();

        glm::mat4 s_torre = fcg::scaling (0.3, 1.2, 0.3);
        
        glm::mat4 tr_torre = fcg::translation (0.0, -0.3, 0.0);
        
        glm::mat4 torre_mm = tr_torre * s_torre * mesh_mm;

        glUniformMatrix4fv(model_loc, 1, GL_FALSE, &torre_mm[0][0]);
        
        glm::mat3 tr_inv_torre = glm::transpose (glm::inverse (glm::mat3 (torre_mm)));
        glUniformMatrix3fv (tr_inv_model_loc, 1, GL_FALSE, &tr_inv_torre[0][0]);

        mesh.draw ();

        // ===========================================================
        // TAPPA 03: Il Rotore (Il mozzo centrale animato)
        // ===========================================================
        
        //Colori rotore
        lights.material_diffuse = {0.3f, 0.15f, 0.05f}; 
        lights.material_ambient = {0.15f, 0.07f, 0.02f};
        lights.send_parameters();

        angolo_rotore += velocita_rotore;

        glm::mat4 s_rotore = fcg::scaling (0.1, 0.1, 0.2);
        
        glm::mat4 r_rotore = glm::rotate (glm::mat4(1.0f), glm::radians(angolo_rotore), glm::vec3(0.0f, 0.0f, 1.0f));
        
        glm::mat4 tr_rotore = fcg::translation (0.0, 0.15, 0.16);
        
        glm::mat4 matrice_base_rotore = tr_rotore * r_rotore;
        
        glm::mat4 rotore_mm = matrice_base_rotore * s_rotore * mesh_mm;

        //invia i dati alla scheda video
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, &rotore_mm[0][0]);
        glm::mat3 tr_inv_rotore = glm::transpose (glm::inverse (glm::mat3 (rotore_mm)));
        glUniformMatrix3fv (tr_inv_model_loc, 1, GL_FALSE, &tr_inv_rotore[0][0]);

        mesh.draw ();

        //Pale e Vele del Mulino
        for (int i = 0; i < 4; i++) {
            
            //Colore Pale
            lights.material_diffuse = {0.3f, 0.15f, 0.05f}; 
            lights.material_ambient = {0.15f, 0.07f, 0.02f};
            lights.send_parameters();

            glm::mat4 s_pala = fcg::scaling (0.8, 0.05, 0.02);
            glm::mat4 tr_pala = fcg::translation (0.4, 0.0, 0.0);
            
            float angolo_pala = i * 90.0f;
            glm::mat4 r_pala = glm::rotate (glm::mat4(1.0f), glm::radians(angolo_pala), glm::vec3(0.0f, 0.0f, 1.0f));
            
            glm::mat4 pala_mm = matrice_base_rotore * r_pala * tr_pala * s_pala * mesh_mm;
            
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, &pala_mm[0][0]);
            glm::mat3 tr_inv_pala = glm::transpose (glm::inverse (glm::mat3 (pala_mm)));
            glUniformMatrix3fv (tr_inv_model_loc, 1, GL_FALSE, &tr_inv_pala[0][0]);
            mesh.draw ();

            //Vela
            //Colore Vela
            lights.material_diffuse = {0.9f, 0.9f, 0.9f}; 
            lights.material_ambient = {0.6f, 0.6f, 0.6f};
            lights.send_parameters();

            glm::mat4 s_vela = fcg::scaling (0.7, 0.2, 0.01);
            glm::mat4 tr_vela = fcg::translation (0.45, -0.12, 0.0);
            
            //la stessa rotazione dell'asta (r_pala)
            glm::mat4 vela_mm = matrice_base_rotore * r_pala * tr_vela * s_vela * mesh_mm;
            
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, &vela_mm[0][0]);
            glm::mat3 tr_inv_vela = glm::transpose (glm::inverse (glm::mat3 (vela_mm)));
            glUniformMatrix3fv (tr_inv_model_loc, 1, GL_FALSE, &tr_inv_vela[0][0]);
            mesh.draw ();
        }
    }