#pragma once
#include "glad/gl.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "include/trackball.hh"
#include "include/hotshaders.hh"
#include "include/matrices.hh"


//////////////
// TAPPA 07 //
//////////////
class Camera
{
    public:
    glm::mat4 v;
    glm::mat4 inv_v;
    glm::mat4 vp;

    glm::vec3 camera_pos   = glm::vec3(0.0f, 1.0f, 5.0f); // Pos partenza
    glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f); // Guarda verso -Z
    glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f, 0.0f);  // Up verso +Y

    float yaw   = -90.0f;
    float pitch = 0.0f;
    float ar = 1.0f; // Aspect ratio

    private:
    GLint camera_pos_loc;

    public:
        Camera (fcg::Shaders& shaders)
        {
            locations (shaders);
            set_window_size (800, 800);
            view_projection ();
        }
    
    void locations (fcg::Shaders& shaders)
    {
        camera_pos_loc = glGetUniformLocation (shaders.program, "camera_pos");
    }

    void set_window_size (int w, int h)
    {
        ar = ((float) w) / (float) h;
        view_projection ();
    }

    //mouse
    void process_mouse (float dx, float dy)
    {
        float sensitivity = 0.2f;
        yaw += dx * sensitivity;
        pitch -= dy * sensitivity; // Invertito perché le Y dello schermo vanno verso il basso

        //evitiamo che si spacchi il collo
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        //Calcolo sul front vector della cam
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camera_front = glm::normalize(front);

        view_projection ();
    }

    void move(float forward_move, float right_move, float up_move)
    {
        //Calcolo del vettore front proiettato sul piano XZ (obbligato al piano XZ)
        glm::vec3 flat_front = glm::normalize(glm::vec3(camera_front.x, 0.0f, camera_front.z));
        
        glm::vec3 right = glm::normalize(glm::cross(flat_front, camera_up));

        //Telecamera movement
        camera_pos += flat_front * forward_move;
        camera_pos += right * right_move;
        camera_pos += camera_up * up_move;

        // ===========================================================
        // TAPPA 11: Collisione col Prato
        // ===========================================================
        
        float limite_prato = 3.0f;
        float altezza_occhi = -0.7f;

        //Controllo del bounding box del prato
        if (camera_pos.x >= -limite_prato && camera_pos.x <= limite_prato &&
            camera_pos.z >= -limite_prato && camera_pos.z <= limite_prato) 
        {
            if (camera_pos.y < altezza_occhi && camera_pos.y > -2.0f) {
                camera_pos.y = altezza_occhi;
            }
        }

        view_projection ();
    }

    //vista
    void view_projection ()
    {
        //matrice di vista
        v = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
        inv_v = glm::inverse(v);

        //matrice di proiezione prospettica
        glm::mat4 pr = glm::perspective(glm::radians(45.0f), ar, 0.1f, 100.0f);

        vp = pr * v;
        glUniform3fv(camera_pos_loc, 1, &camera_pos[0]);
    }
};