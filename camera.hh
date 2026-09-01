#pragma once
#include "glad/gl.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "include/trackball.hh"
#include "include/hotshaders.hh"
#include "include/matrices.hh"

/*class Camera
{
public:
    glm::mat4 v;
    glm::mat4 inv_v;
    glm::mat4 vp;

private:
    // Intrinsic camera parameters
    const float normal_fd = 80.0;
    const float tele_fd =  400.0 / 18.0;
    const float wide_fd = 24 / 18.0;
    float fd; // focal distance
    float ar; // aspect ratio

    // Extrinsic camera parameters
    // xyz, camera position (fixed in world coordinates)
    glm::vec3 camera_pos = {0.0, 0.0, 0.0};
    GLint camera_pos_loc;
    // The Trackball contains the object rotation relative to the fixed camera position
    fcg::Trackball trackball;
    // object distance, relative to the fixed camera position
    float od;

public:
    Camera (fcg::Shaders& shaders)
    {
        locations (shaders);
        lens_normal ();
        set_window_size (800, 800);
        view_projection ();
    }

    void locations (fcg::Shaders& shaders)
    {
        camera_pos_loc = glGetUniformLocation (shaders.program, "camera_pos");
    }

    void set_window_size (int w, int h)
    {
        trackball.set_window_size (w, h);
        ar = ((float) w) / (float) h;
        trackball.set_view (od, 1.0f / (fd * ar));
        view_projection ();
    }

    void start_rotate (float x, float y)
    {
        trackball.start (x, y);
    }

    void stop_rotate ()
    {
        trackball.stop ();
    }

    bool rotate (float x, float y)
    {
        bool moved = trackball.move (x, y);
        if (moved)
            view_projection ();
        return moved;
    }

    void zoom (float dy)
    {
        float ratio = fd / 100.0;
        fd += dy * ratio;
        if (fd < 0.1)
            fd = 0.1;
        trackball.set_view (od, 1.0f / (fd * ar));
        view_projection ();
    }

    void distance (float dy)
    {
        float ratio = od / 100.0;
        od -= dy * ratio; // note: we go in the opposite direction of zoooming
        if (od < 0.5)
            od = 0.5;
        trackball.set_view (od, 1.0f / (fd * ar));
        view_projection ();
    }

    void lens_tele ()
    {
        fd = tele_fd;
        od = tele_fd;
        trackball.set_view (od, 1.0f / (fd * ar));
        view_projection ();
    }

    void lens_normal ()
    {
        fd = normal_fd;
        od = normal_fd;
        trackball.set_view (od, 1.0f / (fd * ar));
        view_projection ();
    }

    void lens_wide ()
    {
        fd = wide_fd;
        od = wide_fd;
        trackball.set_view (od, 1.0f / (fd * ar));
        view_projection ();
    }

    void view_projection ()
    {
        float ncp = od - 4.0; // distance near clip plane
        if (ncp < 0.0001)
            ncp = 0.0001;
        float fcp = od + 4.0; // distance far clip plane

        // rotation matrix from trackball
        glm::mat4 r = trackball.rotation_matrix ();

        // prepare translation matrix
        glm::mat4 tz = fcg::translation (0.0, 0.0, -od);

        // prepare projection matrix
        float a = (fcp + ncp) / (ncp - fcp);       // coefficient 3rd col
        float b = 2.0 * fcp * ncp / (ncp - fcp);   // coefficient 4th col

        glm::mat4 pr = glm::mat4(
                                 fd,  0.0,     0.0,  0.0,    // 1st column
                                 0.0, fd * ar, 0.0,  0.0,    // 2nd column
                                 0.0, 0.0,       a, -1.0,    // 3rd column
                                 0.0, 0.0,       b,  0.0     // 4th column
                                 );

        // Compute VP matrix and update it
        v = tz * r;
        vp = pr * v;
        inv_v = glm::inverse (v);

        glm::vec4 cp4 = {0.0, 0.0, 0.0, 1.0};
        cp4 = inv_v * cp4;
        glm::vec3 cp3 = {cp4.x, cp4.y, cp4.z};
        glUniform3fv(camera_pos_loc, 1, &cp3[0]);
    }
};*/

//Tappa 07
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