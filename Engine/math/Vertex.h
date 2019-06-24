#pragma once

#include "../other/Color.h"
#include "Vec3.h"

struct Vertex {
	Vec3 position;
	Color<> color;

	Vertex(const Vec3& _position = Vec3(0), const bool _randomColors = false, const Color<>& _color = Color<>(255));
};