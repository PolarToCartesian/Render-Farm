#include "Material.h"

Material::Material(const float _reflectivity, const float _shineDamper, const std::array<Color<>, 3>& _colors)
	: reflectivity(_reflectivity), shineDamper(_shineDamper), vertexColors(_colors) {}

Material::Material(const float _reflectivity, const float _shineDamper, const std::string& _textureName)
	: reflectivity(_reflectivity), shineDamper(_shineDamper), textureName(_textureName), isTextured(true) {}