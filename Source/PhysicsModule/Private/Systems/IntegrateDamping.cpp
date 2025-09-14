// Copyright (c) Eric Jeker. All Rights Reserved.

#include "PhysicsModule/Systems/IntegrateDamping.h"

#include "PhysicsModule/Components/Damping.h"
#include "PhysicsModule/Components/RigidBody.h"

namespace {

auto Update() {
  return [](const Damping& d, RigidBody& b) {
    if (b.inverseMass <= 0.f)
      return;

    b.force += -d.coefficient * b.velocity;
  };
}

}  // namespace

void IntegrateDamping::Register(const flecs::world& world) {
  world.system<const Damping, RigidBody>("IntegrateDampingForce").each(Update());
}
