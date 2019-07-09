#pragma once

#include "../../other/Defines.h"
#include "../../other/Color.h"
#include "../../other/pch.h"
#include "Model.h"

struct Box {
	Model model;

	Box(const Vec3& _center, const double sideLength, const bool _randomColor, const Color<>& _color, const Vec3& _rotation = Vec3(0));
};