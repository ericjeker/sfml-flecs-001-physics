// Copyright (c) Eric Jeker. All Rights Reserved.

#include "PhysicsModule/PhysicsModule.h"

#include "PhysicsModule/Components/Acceleration.h"
#include "PhysicsModule/Components/Damping.h"
#include "PhysicsModule/Components/Drag.h"
#include "PhysicsModule/Components/Gravity.h"
#include "PhysicsModule/Components/RigidBody.h"
#include "PhysicsModule/Systems/IntegrateAcceleration.h"
#include "PhysicsModule/Systems/IntegrateDamping.h"
#include "PhysicsModule/Systems/IntegrateDrag.h"
#include "PhysicsModule/Systems/IntegrateGravity.h"
#include "PhysicsModule/Systems/IntegratePhysics.h"

void PhysicsModule::Register(const flecs::world& world) {
  world.component<RigidBody>();
  world.component<Drag>();
  world.component<Gravity>();
  world.component<Damping>();
  world.component<Acceleration>();

  // --- Register Systems ---
  IntegrateGravity::Register(world);
  IntegrateAcceleration::Register(world);
  IntegrateDrag::Register(world);
  IntegrateDamping::Register(world);
  
  // Integrate the accumulated forces
  IntegratePhysics::Register(world);
}
