#include <flecs.h>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <algorithm>
#include <cmath>
#include <flecs/addons/cpp/mixins/pipeline/decl.hpp>

#include "core/components/CircleRenderable.h"
#include "core/components/Physics.h"
#include "core/components/Transform.h"

int main() {
  sf::ContextSettings settings;
  settings.antiAliasingLevel = 4;

  auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "CMake SFML Project", sf::Style::None,
                                 sf::State::Fullscreen, settings);
  window.setFramerateLimit(144);

  // the unique flecs world
  const flecs::world world;

  // circle with 100-pixel radius, green fill
  auto particle = world.entity()
                      .set<CircleRenderable>({})
                      .set<Transform>({{1920 / 2.f, 1080 / 2.f}})
                      .set<Physics>({.damping = 0.98f});
  auto& shape = particle.get_mut<CircleRenderable>().shape;
  shape.setRadius(20.f);
  shape.setPointCount(64);
  shape.setFillColor(sf::Color::Green);
  shape.setOrigin({20.f, 20.f});

  // add our systems to the world
  world.system<Transform, Physics>("PhysicsIntegratorSystem")
      .each([](const flecs::iter& it, size_t, Transform& t, Physics& p) {
        if (p.inverseMass <= 0.f)
          return;

        const float dt = it.delta_time();
        assert(dt > 0.f);

        // integrate the gravity
        p.acceleration += p.gravity;

        // integrate to the velocity considering the damping
        p.velocity *= std::pow(p.damping, dt);
        p.velocity += p.acceleration * dt;

        // integrate to the position
        t.position += p.velocity * dt;

        // clear the acceleration
        p.acceleration = {0.f, 0.f};
      });

  world.system<const CircleRenderable, Transform, Physics>("ScreenBounceSystem")
      .kind(flecs::PostUpdate)
      .each([&window](const CircleRenderable& c, Transform& t, Physics& p) {
        const auto windowSize = window.getSize();
        const auto radius = c.shape.getRadius();

        if (t.position.x - radius < 0 || t.position.x + radius > windowSize.x) {
          p.velocity.x *= -1;
          t.position.x = std::clamp(t.position.x, radius, windowSize.x - radius);
        } else if (t.position.y - radius < 0 || t.position.y + radius > windowSize.y) {
          p.velocity.y *= -1;
          t.position.y = std::clamp(t.position.y, radius, windowSize.y - radius);
        }
      });

  world.system<CircleRenderable, const Transform>("RenderingSystem")
      .kind(flecs::OnStore)
      .each([&window](CircleRenderable c, const Transform t) {
        c.shape.setRotation(sf::degrees(t.rotation));
        c.shape.setPosition(t.position);
        window.draw(c.shape);
      });

  sf::Clock clock;
  while (window.isOpen()) {
    const float elapsed = clock.restart().asSeconds();

    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Escape) {
          window.close();
        }
      }
    }

    window.clear();
    world.progress(elapsed);
    window.display();
  }

  return 0;
}
