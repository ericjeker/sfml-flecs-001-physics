// Copyright (c) Eric Jeker. All Rights Reserved.

#include "PhysicsModule/Systems/IntegrateGravity.h"

#include "PhysicsModule/Components/Gravity.h"
#include "PhysicsModule/Components/RigidBody.h"

namespace {

auto Update() {
  return [](const Gravity& g, RigidBody& b) {
    if (b.inverseMass <= 0.f)
      return;
    
    b.force += g.vector * b.inverseMass;
  };
}

}  // namespace

void IntegrateGravity::Register(const flecs::world& world) {
  world.system<const Gravity, RigidBody>("IntegrateGravity").each(Update());
}
