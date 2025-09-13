#include <SFML/System/Vector2.hpp>

struct Physics {
  // we use inverse mass so it's easy to simulate infinite mass for imovable objects
  float inverseMass = 1.f;
  sf::Vector2f velocity = {0.f, 0.f};
  sf::Vector2f acceleration = {0.f, 0.f};
  float damping = 0.98f;
  // gravity in cm/s
  sf::Vector2f gravity = {0.f, 980.7f};
};
