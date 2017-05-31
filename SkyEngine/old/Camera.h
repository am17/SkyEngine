#pragma once
#include "ICamera.h"

namespace scene
{
	class MovingCameraCommand;

	class Camera : public ICamera
	{
	public:
		Camera();
		~Camera();

		void setPosition(float x, float y, float z) override;
		void setRotation(float x, float y, float z) override;
		XMFLOAT3 getPosition() override;
		XMFLOAT3 getRotation() override;
		XMFLOAT3 getTarget() const override;
		XMFLOAT4X4 getViewMatrix() override;
		XMFLOAT4X4 getReflectionViewMatrix() override;
		void update() override;
		void updateReflectViewMatrix(float height) override;
		void moveForward() override;
		void moveBackward() override;
		void stepRotateLeft() override;
		void stepRotateRight() override;
		void stepRotateUp() override;
		void stepRotateDown() override;
		void stepDown() override;
		void stepUp() override;
		void setProjectionMatrix(XMFLOAT4X4 projectionMatrix) override;
		const BoundingFrustum *getFrustum() const override;
	private:
		XMFLOAT3 m_position;
		XMFLOAT3 m_rotation;
		XMFLOAT3 m_target;
		XMFLOAT4X4 m_viewMatrix;
		XMFLOAT4X4 m_projectionMatrix;
		XMFLOAT4X4 m_reflectViewMatrix;

		float m_moveBackForward;
		float m_moveUpDown;
		float m_moveLeftRight;
		float m_pitch;
		float m_yaw;
		float m_moveSpeed;
		float m_rotateSpeed;

		BoundingFrustum m_frustum;
	};
}

