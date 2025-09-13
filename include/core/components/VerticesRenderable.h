// Copyright (c) Eric Jeker. All Rights Reserved.

#pragma once
#include "SFML/Graphics/VertexArray.hpp"

struct VerticesRenderable {
  std::vector<sf::Vertex> vertices;
  sf::PrimitiveType primitiveType;
};
