#include <flecs.h>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowEnums.hpp>

#include <algorithm>
#include <cmath>

#include "core/components/CircleRenderable.h"
#include "core/components/Physics.h"
#include "core/components/Transform.h"
#include "core/components/VerticesRenderable.h"
#include "core/themes/Nord.h"

namespace {
struct ScreenBoundaries {
  sf::FloatRect bounds;
};

const float SCREEN_PADDING = 5.f;
const float SCREEN_WIDTH = 1920.f;
const float SCREEN_HEIGHT = 1080.f;
}  // namespace

flecs::entity CreateParticleEntity(const flecs::world& world) {
  const auto particle = world.entity()
                            .set<CircleRenderable>({})
                            .set<Transform>({{SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f}})
                            .set<Physics>({.damping = 0.98f});
  auto& shape = particle.get_mut<CircleRenderable>().shape;
  shape.setRadius(20.f);
  shape.setPointCount(64);
  shape.setFillColor(NordTheme::Frost1);
  shape.setOrigin({20.f, 20.f});

  return particle;
}

flecs::entity CreateScreenBorder(const flecs::world& world) {
  const auto border = world.entity().set<VerticesRenderable>({.primitiveType = sf::PrimitiveType::TriangleStrip});
  auto& vertices = border.get_mut<VerticesRenderable>().vertices;
  vertices.push_back({.position = {SCREEN_PADDING, SCREEN_PADDING}, .color = NordTheme::PolarNight1});
  vertices.push_back({.position = {SCREEN_WIDTH - SCREEN_PADDING, SCREEN_PADDING}, .color = NordTheme::PolarNight1});
  vertices.push_back(
      {.position = {SCREEN_WIDTH - SCREEN_PADDING, SCREEN_HEIGHT - SCREEN_PADDING}, .color = NordTheme::PolarNight1});
  vertices.push_back({.position = {SCREEN_PADDING, SCREEN_HEIGHT - SCREEN_PADDING}, .color = NordTheme::PolarNight1});
  vertices.push_back({.position = {SCREEN_PADDING, SCREEN_PADDING}, .color = NordTheme::PolarNight1});

  return border;
}

auto IntegratePhysics() {
  return [](const flecs::iter& it, size_t, Transform& t, Physics& p) {
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
  };
}

auto HandleBoundaryCollision() {
  return [](const flecs::iter& it, size_t, const CircleRenderable& c, Transform& t, Physics& p) {
    const auto screenBounds = it.world().get<ScreenBoundaries>().bounds;
    const auto radius = c.shape.getRadius();

    if (t.position.x - radius < screenBounds.position.x ||
        t.position.x + radius > screenBounds.position.x + screenBounds.size.x) {
      p.velocity.x *= -1;
      t.position.x = std::clamp(t.position.x, screenBounds.position.x + radius,
                                screenBounds.position.x + screenBounds.size.x - radius);
    } else if (t.position.y - radius < screenBounds.position.y ||
               t.position.y + radius > screenBounds.position.y + screenBounds.size.y) {
      p.velocity.y *= -1;
      t.position.y = std::clamp(t.position.y, screenBounds.position.y + radius,
                                screenBounds.position.y + screenBounds.size.y - radius);
    }
  };
}

auto DrawVertices(sf::RenderWindow& window) {
  return [&window](const VerticesRenderable& v) {
    window.draw(v.vertices.data(), v.vertices.size(), v.primitiveType);
  };
}

auto DrawCircleShape(sf::RenderWindow& window) {
  return [&window](CircleRenderable c, const Transform t) {
    c.shape.setRotation(sf::degrees(t.rotation));
    c.shape.setPosition(t.position);
    window.draw(c.shape);
  };
}

int main() {
  sf::ContextSettings settings;
  settings.antiAliasingLevel = 4;

  auto window = sf::RenderWindow(sf::VideoMode({static_cast<unsigned>(1920.f), static_cast<unsigned>(1080.f)}),
                                 "CMake SFML Project", sf::Style::None, sf::State::Windowed, settings);
  window.setFramerateLimit(144);

  // the unique flecs world
  const flecs::world world;

  // --- Define Singletons ---
  world.set<ScreenBoundaries>({sf::FloatRect{{SCREEN_PADDING, SCREEN_PADDING},
                                             {SCREEN_WIDTH - 2 * SCREEN_PADDING, SCREEN_HEIGHT - 2 * SCREEN_PADDING}}});

  // --- Add Entities ---
  CreateScreenBorder(world);
  CreateParticleEntity(world);

  // --- Add Systems ---
  world.system<Transform, Physics>("PhysicsIntegratorSystem").each(IntegratePhysics());

  world.system<const CircleRenderable, Transform, Physics>("ScreenBounceSystem")
      .kind(flecs::PostUpdate)
      .each(HandleBoundaryCollision());

  world.system<const VerticesRenderable>("VerticesRenderingSystem").kind(flecs::OnStore).each(DrawVertices(window));

  world.system<CircleRenderable, const Transform>("CircleRenderingSystem")
      .kind(flecs::OnStore)
      .each(DrawCircleShape(window));

  // --- Run the game loop ---
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

    window.clear(NordTheme::PolarNight4);
    world.progress(elapsed);
    window.display();
  }

  return 0;
}
