#include "glad/gl.h"
#include <SFML/Window.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>
#include <cstdlib>

#include "include/hotshaders.hh"
#include "scene.hh"
#include "input.hh"
#include "rawmouse.hh"


/////////////////////////////
// Window and OpenGL setup //
/////////////////////////////

class Setup
{
public:
    static const int window_width = 800;
    static const int window_height = 800;

    sf::Window* window;

    Setup ()
    {
        sf::ContextSettings settings;
        settings.depthBits = 32;
        settings.stencilBits = 8;
        settings.antiAliasingLevel = 4;
        settings.attributeFlags = sf::ContextSettings::Attribute::Core;
        settings.majorVersion = 4;
        settings.minorVersion = 1;

        window = new sf::Window (
            sf::VideoMode::getDesktopMode(),
            "SFML + OpenGL",
            sf::Style::None,
            sf::State::Windowed,
            settings
        );

        window->setPosition(sf::Vector2i(0, 0));
        auto desktop = sf::VideoMode::getDesktopMode().size;

        window->setFramerateLimit(60);
        window->setVerticalSyncEnabled (true);

        if (!window->setActive (true)) {
            std::cerr << "Failure: error during SFML OpenGL Activation." << std::endl;
            exit (1);
        }
        sf::ContextSettings gotten = window->getSettings ();

        std::cout << "depth bits: " << gotten.depthBits << std::endl;
        std::cout << "stencil bits: " << gotten.stencilBits << std::endl;
        std::cout << "antialiasing level: " << gotten.antiAliasingLevel << std::endl;
        std::cout << "SFML GL version: " << gotten.majorVersion << "." << gotten.minorVersion << std::endl;

        int version = gladLoadGL (sf::Context::getFunction);
        if (!version) {
            std::cerr << "Failure: error during glad loading." << std::endl;
            exit (1);
        }
        std::cout << "GLAD GL version: " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;
    }

    ~Setup ()
    {
        delete window;
    }
};

//////////
// Main //
//////////

int main (int argc, char* argv[])
{
    // mandatory command line argument: mesh file to open
    std::string meshfile = "";
    if (argc > 1)
        meshfile = argv[1];
    else {
        std::cout << "Usage: "<<argv[0]<< " meshfile\n";
        exit (1);
    }


    //// Startup ////

    Setup setup;
    sf::Window& window = *setup.window;
    window.setMouseCursorVisible(false);
    window.setMouseCursorGrabbed(true);

    RawMouse rawmouse;
    //centratura mouse
    rawmouse.setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2) + window.getPosition());
    
    fcg::Shaders shaders ("shader_flat.vert", "shader_flat.frag");
    shaders.use ();

    Scene scene (meshfile, shaders);

    glEnable (GL_CULL_FACE);
    glCullFace (GL_BACK);

    glEnable (GL_DEPTH_TEST);


    //// Main Loop ////

    bool running = true;
    while (running)
    {
        while (const std::optional event = window.pollEvent ())
        {
            if (event->is<sf::Event::Closed> ())
                running = false;
            else if (const auto* resized = event->getIf<sf::Event::Resized> ())
                handle (*resized, scene.camera);
            else if (const auto* key_pressed = event->getIf<sf::Event::KeyPressed> ())
                handle (*key_pressed, shaders, scene);
            else if (const auto* mouse_moved = event->getIf<sf::Event::MouseMovedRaw> ())
                rawmouse.event(*mouse_moved);
        }

        handle_realtime_input(scene, rawmouse);

        scene.draw ();
        window.display ();
    }

    return 0;
}
