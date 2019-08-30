#pragma once

#include "../other/Color.h"
#include "Vec3.h"

struct Vertex {
	Vec3 position;
	Vec3 normal;

	Vertex(const Vec3& _position = Vec3(0));
};