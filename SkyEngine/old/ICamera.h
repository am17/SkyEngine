#pragma once
#include <DirectXMath.h>
#include <DirectXCollision.h>

using namespace DirectX;

namespace scene
{
	class ICamera
	{
	public:
		virtual ~ICamera() {}
		virtual void setPosition(float x, float y, float z) = 0;
		virtual void setRotation(float x, float y, float z) = 0;
		virtual XMFLOAT3 getPosition() = 0;
		virtual XMFLOAT3 getRotation() = 0;
		virtual XMFLOAT3 getTarget() const = 0;
		virtual XMFLOAT4X4 getViewMatrix() = 0;
		virtual XMFLOAT4X4 getReflectionViewMatrix() = 0;
		virtual void update() = 0;
		virtual void updateReflectViewMatrix(float height) = 0;
		virtual void moveForward() = 0;
		virtual void moveBackward() = 0;
		virtual void stepRotateLeft() = 0;
		virtual void stepRotateRight() = 0;
		virtual void stepRotateUp() = 0;
		virtual void stepRotateDown() = 0;
		virtual void stepDown() = 0;
		virtual void stepUp() = 0;
		virtual void setProjectionMatrix(XMFLOAT4X4 projectionMatrix) = 0;
		virtual const BoundingFrustum *getFrustum() const = 0;
	};
}
