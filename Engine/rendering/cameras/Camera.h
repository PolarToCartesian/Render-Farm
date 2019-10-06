#pragma once

#include "../../math/Vec3.h"

#include <array>

class Camera {
	protected:
		Vec3 position, rotation;

		float fov, zNear, zFar;
		uint16_t width, height;

		Mat4x4 rotationXMatrix, rotationYMatrix, rotationZMatrix, projectionMatrix;

		bool wasModified = true;

	public:
		Camera(const uint16_t _width, const uint16_t _height, const float _fov = 90.f, const float _zNear = 0.1f, const float _zFar = 1000.f);

		Vec3 getPosition() const;
		Vec3 getRotation() const;
		float getFov() const;
		float getZNear() const;
		float getZFar() const;
		uint16_t getWidth() const;
		uint16_t getHeight() const;
		
		void setPosition(const Vec3& _position);
		void setRotation(const Vec3& _rotation);
		void setFov(const float _fov);
		void setZNear(const float _zNear);
		void setZFar(const float _zFar);
		void setWidth(const uint16_t _width);
		void setHeight(const uint16_t _height);

		virtual void calculateMatrices() = 0;

		virtual void applyProjection(Vec3& _vertex) const = 0;

		void applyTranslation(Vec3& _vertex) const;

		void applyRotation(Vec3& _vertex) const;

		bool isTriangleFacingCamera(const std::array<Vec3, 3>& _points, const Vec3& _triangleSurfaceNormal) const;
};