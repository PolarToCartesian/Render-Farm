#include "Vertex.h"

Vertex::Vertex(const Vec3& _position, const Vec3& _textureCoord)
	: position(_position), textureCoord(_textureCoord)
{
	this->textureCoord = Vec3(rand() % RAND_MAX, rand() % RAND_MAX);
}