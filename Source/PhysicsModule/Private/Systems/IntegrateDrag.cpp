// Copyright (c) Eric Jeker. All Rights Reserved.

#include "PhysicsModule/Systems/IntegrateDrag.h"

#include "PhysicsModule/Components/Drag.h"
#include "PhysicsModule/Components/RigidBody.h"

namespace {

auto Update() {
  return [](const Drag& d, RigidBody& b) {
    if (b.inverseMass <= 0.f)
      return;

    const float speed = b.velocity.length();
    if (speed <= 0.f)
      return;

    const float drag = d.k1 * speed + d.k2 * speed * speed;
    if (drag <= 0.f)
      return;
    
    b.force += -drag * b.velocity.normalized();
  };
}

}  // namespace

void IntegrateDrag::Register(const flecs::world& world) {
  world.system<const Drag, RigidBody>("IntegrateDragSystem").each(Update());
}
