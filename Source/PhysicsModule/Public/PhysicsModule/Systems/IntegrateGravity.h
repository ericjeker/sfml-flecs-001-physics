// Copyright (c) Eric Jeker. All Rights Reserved.

#pragma once

#include <flecs.h>

struct IntegrateGravity {
  static void Register(const flecs::world& world);
};
