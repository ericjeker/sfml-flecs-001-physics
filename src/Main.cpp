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
#include "core/components/ScreenBoundaries.h"
#include "core/components/Transform.h"
#include "core/components/VerticesRenderable.h"
#include "core/themes/Nord.h"

namespace {
constexpr float SCREEN_PADDING = 5.f;
constexpr float SCREEN_WIDTH = 1920.f;
constexpr float SCREEN_HEIGHT = 1080.f;
constexpr float PARTICLE_RADIUS = 20.f;
constexpr sf::Vector2f GRAVITY = {0.f, 9800.f};
constexpr float RESTITUTION = 0.9f;

struct MouseState {
  sf::Vector2i startPosition;
};

struct LifeTime {
  float seconds = 1.f;
};
}  // namespace

flecs::entity CreateParticleEntity(const flecs::world& world) {
  const auto particle = world.entity()
                            .set<CircleRenderable>({})
                            .set<Transform>({{SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f}})
                            .set<Physics>({.damping = 0.98f, .gravity = GRAVITY});
  auto& shape = particle.get_mut<CircleRenderable>().shape;
  shape.setRadius(PARTICLE_RADIUS);
  shape.setPointCount(16);
  shape.setFillColor(NordTheme::Frost1);
  shape.setOrigin({PARTICLE_RADIUS, PARTICLE_RADIUS});

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
    bool collided = false;

    if (t.position.x - radius < screenBounds.position.x ||
        t.position.x + radius > screenBounds.position.x + screenBounds.size.x) {
      collided = true;
      p.velocity.x *= -1;
      t.position.x = std::clamp(t.position.x, screenBounds.position.x + radius,
                                screenBounds.position.x + screenBounds.size.x - radius);
    } else if (t.position.y - radius < screenBounds.position.y ||
               t.position.y + radius > screenBounds.position.y + screenBounds.size.y) {
      collided = true;
      p.velocity.y *= -1;
      t.position.y = std::clamp(t.position.y, screenBounds.position.y + radius,
                                screenBounds.position.y + screenBounds.size.y - radius);
    }

    // Restitution
    if (collided)
      p.velocity *= RESTITUTION;
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

auto ProcessLifeTime() {
  return [](const flecs::entity& e, LifeTime& lt) {
    lt.seconds -= e.world().delta_time();

    if (lt.seconds <= 0.f) {
      e.destruct();
    }
  };
}

void ShotParticleOnMouseReleased(const flecs::world& world, const sf::Event::MouseButtonReleased* mouseReleased) {
  // Record the position of the release
  const auto startPosition = world.get<MouseState>().startPosition;
  const auto releasePosition = mouseReleased->position;

  // Calculate the vector between the two clicks
  auto delta = releasePosition - startPosition;
  delta *= 10;

  // Generate a particle and give it that acceleration
  const auto particle = CreateParticleEntity(world);
  particle.set<LifeTime>({.seconds = 5.f});
  particle.get_mut<Transform>().position =
      sf::Vector2f{static_cast<float>(startPosition.x), static_cast<float>(startPosition.y)};
  particle.get_mut<Physics>().velocity = sf::Vector2f{static_cast<float>(delta.x), static_cast<float>(delta.y)};

  // Clean-up
  world.remove<MouseState>();
}

void UpdateThrowLine(const flecs::world& world, const sf::Event::MouseMoved* mouseMoved) {
  if (!world.has<MouseState>())
    return;

  const auto& [startPosition] = world.get<MouseState>();
  const auto& currentPosition = mouseMoved->position;
  world.set<VerticesRenderable>({
      .primitiveType = sf::PrimitiveType::Lines,
  });
  world.get_mut<VerticesRenderable>().vertices.push_back(
      sf::Vertex{.position = sf::Vector2f(startPosition), .color = NordTheme::Frost1});
  world.get_mut<VerticesRenderable>().vertices.push_back(
      sf::Vertex{.position = sf::Vector2f(currentPosition), .color = NordTheme::Frost1});
}

int main() {
  sf::ContextSettings settings;
  settings.antiAliasingLevel = 4;

  auto window =
      sf::RenderWindow(sf::VideoMode({static_cast<unsigned>(SCREEN_WIDTH), static_cast<unsigned>(SCREEN_HEIGHT)}),
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

  world.system<LifeTime>("LifeTimeSystem").each(ProcessLifeTime());

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
        } else if (keyPressed->code == sf::Keyboard::Key::R) {
          // Restart the simulation
          world.query<Transform, Physics>().each([](Transform& t, Physics& p) {
            t.position = {SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f};
            p.velocity = {0.f, 0.f};
          });
        }
      } else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
        UpdateThrowLine(world, mouseMoved);
      } else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
        // Record the position of the initial click
        world.set<MouseState>({.startPosition = mousePressed->position});
      } else if (auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
        ShotParticleOnMouseReleased(world, mouseReleased);
        world.remove<VerticesRenderable>();
      }
    }

    window.clear(NordTheme::PolarNight4);
    world.progress(elapsed);
    window.display();
  }

  return 0;
}
