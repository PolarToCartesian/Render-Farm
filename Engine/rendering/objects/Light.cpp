#include "Light.h"

// Light Struct

Light::Light(const Vec3& _position, const Color& _color, const double& _intensity) : position(_position), color(_color), intensity(_intensity) {}

// Engine Namespace

std::array<Color, 3> EN::LIGHT::applyLightingToVertices(const Vec3 * _vertices, const Color * _colorOfVertices, const Vec3 & _triangleSurfaceNormal, std::vector<Light> & _lights) {
	std::array<Color, 3> output{ { Color(0), Color(0), Color(0) } };

	for (unsigned char v = 0; v < 3; v++) {
		double totalVertexBrightness = 0.f;

		for (unsigned int lightIndex = 0; lightIndex < _lights.size(); lightIndex++) {
			Vec3 vertexToLight = _lights[lightIndex].position - _vertices[v];

			double dotProduct = EN::VECTOR3D::dotProduct(vertexToLight, _triangleSurfaceNormal);

			if (dotProduct < 0) dotProduct = 0;

			totalVertexBrightness += (dotProduct * _lights[lightIndex].intensity) / std::pow(vertexToLight.getLength(), 2);
		}

		totalVertexBrightness = EN::MATH::constrain(totalVertexBrightness, 0, 1);

		output[v] = (_colorOfVertices[v] * totalVertexBrightness);
	}

	return output;
}