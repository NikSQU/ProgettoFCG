#include "input.hh"
#include "scene.hh"

void handle (const sf::Event::Resized& resized, Camera& camera)
{
    glViewport (0, 0, resized.size.x, resized.size.y);
    camera.set_window_size (resized.size.x, resized.size.y);
}

void handle (const sf::Event::KeyPressed& key, fcg::Shaders& shaders, Scene& scene)
{
    switch (key.scancode) {
    case sf::Keyboard::Scancode::Escape:
        exit (0);
    case sf::Keyboard::Scancode::G:
        shaders.reload ("shader_gouraud.vert", "shader_gouraud.frag");
        shaders.use ();
        scene.locations (shaders);
        scene.update_all ();
        return;
    case sf::Keyboard::Scancode::P:
        shaders.reload ("shader_phong.vert", "shader_phong.frag");
        shaders.use ();
        scene.locations (shaders);
        scene.update_all ();
        return;
    case sf::Keyboard::Scancode::F:
        shaders.reload ("shader_flat.vert", "shader_flat.frag");
        shaders.use ();
        scene.locations (shaders);
        scene.update_all ();
        return;
    /*case sf::Keyboard::Scancode::C:
        shaders.reload ("shader_normals.vert", "shader_normals.frag");
        shaders.use ();
        scene.locations (shaders);
        scene.update_all ();
        return;*/
    case sf::Keyboard::Scancode::N:
        scene.camera.lens_normal ();
        scene.lights.send_position_relative (scene.camera.inv_v);
        return;
    case sf::Keyboard::Scancode::T:
        scene.camera.lens_tele ();
        scene.lights.send_position_relative (scene.camera.inv_v);
        return;
    case sf::Keyboard::Scancode::W:
        scene.camera.lens_wide ();
        scene.lights.send_position_relative (scene.camera.inv_v);
        return;
    case sf::Keyboard::Scancode::Up:
        if (scene.velocita_rotore < 5.0f) {
            scene.velocita_rotore += 0.2f;
        }
        return;
    case sf::Keyboard::Scancode::Down:
        if (scene.velocita_rotore > 0.0f) {
            scene.velocita_rotore -= 0.2f;
        }
        if (scene.velocita_rotore < 0.0f) scene.velocita_rotore = 0.0f;  //Per evitare che la velocità arrivi a -0.2
        return;
    case sf::Keyboard::Scancode::Space:
        if (scene.velocita_rotore == 0.0f) {
            // PLAY
            scene.velocita_rotore = scene.velocita_salvata;
        } else {
            // PAUSA
            scene.velocita_salvata = scene.velocita_rotore;
            scene.velocita_rotore = 0.0f;
        }
        return;
    default:
        return;
    }
}

/*
void handle (const sf::Event::MouseMoved& mouse_moved, Scene& scene)
{
    float x = mouse_moved.position.x;
    float y = mouse_moved.position.y;

    static float prev_y = 0;
    float dy = y - prev_y; 
    prev_y = y;

    if (scene.camera.rotate (x, y))
        scene.lights.send_position_relative (scene.camera.inv_v);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
        scene.camera.zoom (dy);
        scene.lights.send_position_relative (scene.camera.inv_v);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt)) {
        scene.camera.distance (dy);
        scene.lights.send_position_relative (scene.camera.inv_v);
    }
}
*/

void handle (const sf::Event::MouseMoved& mouse_moved, Scene& scene)
{
    static float prev_x = mouse_moved.position.x;
    static float prev_y = mouse_moved.position.y;
    
    float dx = mouse_moved.position.x - prev_x;
    float dy = mouse_moved.position.y - prev_y;
    
    prev_x = mouse_moved.position.x;
    prev_y = mouse_moved.position.y;
}


void handle (const sf::Event::MouseButtonPressed& mouse_pressed, Camera& camera)
{
    if (mouse_pressed.button == sf::Mouse::Button::Left) {
        float x = mouse_pressed.position.x;
        float y = mouse_pressed.position.y;
        camera.start_rotate (x, y);
    }
}

void handle (const sf::Event::MouseButtonReleased& mouse_released, Camera& camera)
{
    if (mouse_released.button == sf::Mouse::Button::Left) {
        camera.stop_rotate ();
    }
}