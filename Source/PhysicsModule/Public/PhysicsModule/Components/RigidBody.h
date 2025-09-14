#include <SFML/System/Vector2.hpp>

struct RigidBody {
  // we use inverse mass so it's easy to simulate infinite mass for immovable objects
  float inverseMass = 1.f;
  sf::Vector2f velocity = {0.f, 0.f};
  sf::Vector2f force = {0.f, 0.f};
};
