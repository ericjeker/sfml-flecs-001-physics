// Copyright (c) Eric Jeker. All Rights Reserved.

#include "PhysicsModule/Systems/IntegrateAcceleration.h"

#include "PhysicsModule/Components/Acceleration.h"
#include "PhysicsModule/Components/RigidBody.h"

namespace {

auto Update() {
  return [](Acceleration& a, RigidBody& b) {
    if (b.inverseMass <= 0.f)
      return;

    b.force += a.vector * b.inverseMass;

    // Reset acceleration
    a.vector = {0.f, 0.f};
  };
}

}  // namespace

void IntegrateAcceleration::Register(const flecs::world& world) {
  world.system<Acceleration, RigidBody>("IntegrateAcceleration").each(Update());
}
