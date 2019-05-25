#include "Light.h"

// Light Struct

Light::Light(const Vector3D& _position, const Color& _color, const TYPE& _intensity) : position(_position), color(_color), intensity(_intensity) {}

// Engine Namespace

namespace EN {
	namespace LIGHT {
		void applyLightingToVertices(const Vector3D* _vertices, const Color* _colorOfVertices, const Vector3D& _triangleSurfaceNormal, std::vector<Light>& _lights, Color _output[3]) {
			for (unsigned char v = 0; v < 3; v++) {
				TYPE totalVertexBrightness = 0.f;

				for (unsigned int lightIndex = 0; lightIndex < _lights.size(); lightIndex++) {
					Vector3D vertexToLight = _lights[lightIndex].position - _vertices[v];

					TYPE dotProduct = EN::VECTOR3D::dotProduct(vertexToLight, _triangleSurfaceNormal);

					if (dotProduct < 0) dotProduct = 0;

					totalVertexBrightness += (dotProduct * _lights[lightIndex].intensity) / std::pow(vertexToLight.getLength(), 2);
				}

				totalVertexBrightness = EN::UTIL::constrain(totalVertexBrightness, 0, 1);

				_output[v] = (_colorOfVertices[v] * totalVertexBrightness);
			}
		}
	};
};