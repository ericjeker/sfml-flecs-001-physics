#include <SFML/Graphics/Vertex.hpp>

struct Transform {
  sf::Vector2f position;
  sf::Vector2f scale = {1.f, 1.f};
  float rotation = 0.f;
};
