#pragma once

#include "Vec3.h"
#include "../other/Color.h"

struct Vertex {
	Vec3 position;
	Vec3 textureCoord;

	Vertex(const Vec3& _position = Vec3(0), const Vec3& _textureCoord = Vec3(0));
};