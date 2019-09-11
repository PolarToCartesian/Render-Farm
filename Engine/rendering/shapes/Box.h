#pragma once

#include "Model.h"
#include "../../other/Color.h"
#include "../../other/Defines.h"

struct Box : Model {
	Box(const Vec3& _center, const float sideLength, const std::string& _material, const Vec3& _rotationMidPoint = Vec3(), const Vec3& _rotation = Vec3(0), const float _reflectivity = 0.25);
};