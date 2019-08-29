#include "Vertex.h"

Vertex::Vertex(const Vec3& _position, const bool _randomColors, const Color<>& _color)
	: position(_position) {
	if (_randomColors) {
		using MATH::randomInt;

		this->color = Color<>(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255));
	} else {
		this->color = _color;
	}
}