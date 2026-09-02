#pragma once
#include <SFML/Window/Event.hpp>
#include "include/rawmouse.hh"

class Camera;
class Scene;
namespace fcg { class Shaders; }

void handle (const sf::Event::Resized& resized, Camera& camera);
void handle (const sf::Event::KeyPressed& key, fcg::Shaders& shaders, Scene& scene);
void handle (const sf::Event::MouseMoved& mouse_moved, Scene& scene);
void handle (const sf::Event::MouseButtonPressed& mouse_pressed, Camera& camera);
void handle (const sf::Event::MouseButtonReleased& mouse_released, Camera& camera);
void handle_realtime_input(Scene& scene, fcg::RawMouse& rawmouse);