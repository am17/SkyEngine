#include "stdafx.h"
#include "Camera.h"

namespace scene
{
	Camera::Camera()
		: m_moveBackForward(0), 
		m_moveUpDown(0),
		m_moveLeftRight(0), 
		m_pitch(0), 
		m_yaw(0),
		m_moveSpeed(0.1f),
		m_rotateSpeed(0.005f)
	{
		m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);

		m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);

		XMVECTOR position = XMLoadFloat3(&m_position);
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMVECTOR lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

		XMMATRIX viewMat = XMMatrixLookAtLH(position, lookAt, up);

		XMStoreFloat4x4(&m_viewMatrix, viewMat);
	}

	Camera::~Camera()
	{

	}

	void Camera::setPosition(float x, float y, float z)
	{
		m_position = XMFLOAT3(x, y, z);

	}
	void Camera::setRotation(float x, float y, float z)
	{
		m_rotation = XMFLOAT3(x, y, z);
	}
	XMFLOAT3 Camera::getPosition()
	{
		return m_position;
	}
	XMFLOAT3 Camera::getRotation()
	{
		return m_rotation;
	}

	XMFLOAT3 Camera::getTarget() const
	{
		return m_target;
	}

	void Camera::update()
	{
		float pitch = m_rotation.x * 0.0174532925f;
		float yaw = m_rotation.y * 0.0174532925f;
		float roll = m_rotation.z * 0.0174532925f;

		pitch += m_pitch;
		yaw += m_yaw;

		XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(yaw, pitch, roll);

		m_rotation.x = m_pitch;
		m_rotation.y = m_yaw;

		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMVECTOR lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

		lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
		up = XMVector3TransformCoord(up, rotationMatrix);

		m_position.y += m_moveUpDown;

		XMVECTOR position = XMLoadFloat3(&m_position);

		XMMATRIX RotateYTempMatrix;
		RotateYTempMatrix = XMMatrixRotationY(pitch);

		XMVECTOR camForward = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), RotateYTempMatrix);
		XMVECTOR camRight = XMVector3TransformCoord(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), RotateYTempMatrix);

		position += m_moveLeftRight * camRight;
		position += m_moveBackForward * camForward;

		m_moveBackForward = 0.0f;
		m_moveLeftRight = 0.0f;
		m_moveUpDown = 0.0f;

		lookAt = position + lookAt;

		XMMATRIX viewMat = XMMatrixLookAtLH(position, lookAt, up);

		XMStoreFloat3(&m_position, position);
		XMStoreFloat3(&m_target, lookAt);
		XMStoreFloat4x4(&m_viewMatrix, viewMat);

		XMVECTOR Orientation = XMQuaternionRotationMatrix(rotationMatrix);

		XMFLOAT4 orientQuaternion;
		XMStoreFloat4(&orientQuaternion, Orientation);

		//XMMATRIX projectMatrix = XMLoadFloat4x4(&m_projectionMatrix);

		//XMMATRIX WVP = rotationMatrix * XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

		//m_frustum.Transform(m_frustum, WVP);

		//m_frustum.Transform(m_frustum, 1, Orientation, XMVectorSet(m_position.x, m_position.y, m_position.z, 0));

		m_frustum = BoundingFrustum(m_position, orientQuaternion, m_frustum.RightSlope, m_frustum.LeftSlope, m_frustum.TopSlope, m_frustum.BottomSlope, 0.01, 1000);
	}

	void Camera::updateReflectViewMatrix(float height)
	{
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		XMVECTOR position = XMVectorSet(m_position.x, -m_position.y + (height * 2.0f), m_position.z, 0.0f);

		float radians = m_rotation.y * 0.0174532925f;

		XMVECTOR lookAt = XMVectorSet(sinf(radians) + m_position.x, position.m128_f32[1], cosf(radians) + m_position.z, 0.0f);

		XMMATRIX viewMat = XMMatrixLookAtLH(position, lookAt, up);

		XMStoreFloat4x4(&m_reflectViewMatrix, viewMat); 
	}

	XMFLOAT4X4 Camera::getViewMatrix()
	{
		return m_viewMatrix;
	}

	XMFLOAT4X4 Camera::getReflectionViewMatrix()
	{
		return m_reflectViewMatrix;
	}

	void Camera::moveForward()
	{
		m_moveBackForward += m_moveSpeed;
	}

	void Camera::moveBackward()
	{
		m_moveBackForward -= m_moveSpeed;
	}

	void Camera::stepRotateLeft()
	{
		m_pitch -= m_rotateSpeed;
	}

	void Camera::stepRotateRight()
	{
		m_pitch += m_rotateSpeed;
	}

	void Camera::stepRotateUp()
	{
		m_yaw -= m_rotateSpeed;
	}
	void Camera::stepRotateDown()
	{
		m_yaw += m_rotateSpeed;
	}

	void Camera::stepDown()
	{
		m_moveUpDown -= m_moveSpeed;
	}
	void Camera::stepUp()
	{
		m_moveUpDown += m_moveSpeed;
	}

	void Camera::setProjectionMatrix(XMFLOAT4X4 projectionMatrix)
	{
		m_projectionMatrix = projectionMatrix;

		XMMATRIX projMatrix = XMLoadFloat4x4(&m_projectionMatrix);

		BoundingFrustum::CreateFromMatrix(m_frustum, projMatrix);


		/*XMMATRIX ViewMatrixLH = XMLoadFloat4x4(&m_viewMatrix);
		XMVECTOR Determinant;
		XMMATRIX InvViewMatrixLH = XMMatrixInverse(&Determinant, ViewMatrixLH);
		m_frustum = BoundingFrustum();
		m_frustum.CreateFromMatrix(m_frustum, projMatrix);
		m_frustum.Transform(m_frustum, InvViewMatrixLH);*/
	}

	const BoundingFrustum *Camera::getFrustum() const
	{
		return &m_frustum;
	}
}
