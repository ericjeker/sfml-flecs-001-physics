// Copyright (c) Eric Jeker. All Rights Reserved.

#pragma once

#include "flecs.h"

struct PhysicsModule {
  static void Register(const flecs::world& world);
};
