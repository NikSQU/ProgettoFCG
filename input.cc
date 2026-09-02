#include "input.hh"
#include "scene.hh"
#include <SFML/Window/Keyboard.hpp>

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
    case sf::Keyboard::Scancode::M:
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


//////////////
// TAPPA 09 //
//////////////

void handle_realtime_input(Scene& scene, fcg::RawMouse& rawmouse)
{
    sf::Vector2f m_delta = rawmouse.delta();

    if(m_delta.x != 0.0f || m_delta.y != 0.0f) {
        scene.camera.process_mouse(m_delta.x, m_delta.y);
        scene.lights.send_position_relative(scene.camera.inv_v);
    }

    float speed = 0.01f;
    float fwd = 0.0f, rgt = 0.0f, up = 0.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) fwd += speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) fwd -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) rgt += speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) rgt -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space)) up += speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift)) up -= speed;

    if (fwd != 0.0f || rgt != 0.0f || up != 0.0f) {
        scene.camera.move(fwd, rgt, up);
        scene.lights.send_position_relative(scene.camera.inv_v);
    }
}