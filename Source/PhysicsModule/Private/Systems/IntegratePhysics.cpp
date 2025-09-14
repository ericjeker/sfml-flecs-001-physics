// Copyright (c) Eric Jeker. All Rights Reserved.

#include "PhysicsModule/Systems/IntegratePhysics.h"

#include "Core/Components/Transform.h"
#include "PhysicsModule/Components/RigidBody.h"

namespace {

auto Update() {
  return [](const flecs::iter& it, size_t, Transform& t, RigidBody& p) {
    if (p.inverseMass <= 0.f) {
      p.force = {0.f, 0.f};
      return;
    }

    const float dt = it.delta_time();
    assert(dt > 0.f);

    // integrate to the velocity considering the damping
    p.velocity += p.force * p.inverseMass * dt;

    // integrate to the position
    t.position += p.velocity * dt;

    // reset forces
    p.force = {0.f, 0.f};
  };
}

}  // namespace

void IntegratePhysics::Register(const flecs::world& world) {
  world.system<Transform, RigidBody>("PhysicsIntegratorSystem").kind(flecs::PostUpdate).each(Update());
}
