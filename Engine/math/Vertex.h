#pragma once

#include "Vec3.h"
#include "../other/Color.h"

struct Vertex {
	Vec3 position;

	Vertex(const Vec3& _position = Vec3(0));
};