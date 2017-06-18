#include "stdafx.h"
#include "Camera.h"
#include <algorithm>


Camera::Camera(int screenWidth, int screenHeight, float nearZ, float farZ)
	:_position(0.0f, 1.0f, -2.0f),
	_target(0.0f, 0.0f, 0.0f),
	_aspectRatio(screenWidth / screenHeight),
	_nearZ(nearZ),
	_farZ(farZ),
	m_ViewProj(new XMFLOAT4X4()),
	m_moveSpeed(0.2f),
	m_moveBackForward(0),
	moveLeftRight(0),
	m_pitch(0),
	m_yaw(0)
{
	XMMATRIX projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, _aspectRatio, _nearZ, _farZ);

	XMStoreFloat4x4(&m_Projection, projection);

	XMVECTOR cameraPos = XMLoadFloat3(&_position);

	XMVECTOR cameraLookAt = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XMMatrixIdentity());
	cameraLookAt = XMVector3Normalize(cameraLookAt);

	cameraLookAt = cameraPos + cameraLookAt;

	XMStoreFloat3(&_target, cameraLookAt);
}


Camera::~Camera()
{
	if (m_ViewProj)
	{
		delete m_ViewProj;
		m_ViewProj = nullptr;
	}
}

void Camera::update()
{
	XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, 0);

	XMVECTOR cameraPos = XMLoadFloat3(&_position);
	XMVECTOR cameraLookAt = XMLoadFloat3(&_target);

	cameraLookAt = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	cameraLookAt = XMVector3Normalize(cameraLookAt);

	XMVECTOR camRight = XMVector3TransformCoord(DefaultRight, camRotationMatrix);
	XMVECTOR camForward = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	XMVECTOR camUp = XMVector3Cross(camForward, camRight);

	cameraPos += moveLeftRight*camRight;
	cameraPos += m_moveBackForward * camForward;

	m_moveBackForward = 0.0f;

	cameraLookAt = cameraPos + cameraLookAt;

	XMMATRIX view = XMMatrixLookAtLH(cameraPos, cameraLookAt, camUp);

	XMStoreFloat4x4(&m_View, view);

	XMMATRIX proj = XMLoadFloat4x4(&m_Projection);

	XMStoreFloat4x4(m_ViewProj, view * proj);
	XMStoreFloat3(&_position, cameraPos);
	XMStoreFloat3(&_target, cameraLookAt);

	notifyObservers();
}

void Camera::setPosition(float x, float y, float z)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;
}
XMFLOAT3 Camera::getPosition() const
{
	return _position;
}
void Camera::setTarget(float x, float y, float z)
{
	_target.x = x;
	_target.y = y;
	_target.z = z;
}
XMFLOAT3 Camera::getTarget() const
{
	return _target;
}

XMFLOAT4X4 Camera::getView() const
{
	return m_View;
}
XMFLOAT4X4 Camera::getProjection() const
{
	return m_Projection;
}
XMFLOAT4X4 *Camera::getViewProjection() const
{
	return m_ViewProj;
}

void Camera::registerObserver(Component* component)
{
	_observers.push_back(component);
}

void Camera::notifyObservers()
{
	vector<Component*>::iterator iterator;

	for (iterator = _observers.begin(); iterator != _observers.end(); ++iterator)
	{
		Entity* entity = static_cast<Entity*>(*iterator);

		MeshComponent* grComp = entity->getMeshComponent();

		grComp->updateViewProj(m_ViewProj);
	}
}

void Camera::moveForward()
{
	m_moveBackForward += m_moveSpeed;
}

void Camera::moveBackward()
{
	m_moveBackForward -= m_moveSpeed;
}

void Camera::rotate()
{
	//XMVECTOR delta = XMVectorSet(float(mouse.x), float(mouse.y), 0.f) * 0.004f;
	XMVECTOR delta = XMVectorSet(float(_mState.lX), float(_mState.lY), 0.f, 0.f) * 0.004f;
	//XMVECTOR delta = XMVectorSet(5, 0, 0.f, 0.f) * 0.004f;

	m_pitch += delta.m128_f32[1];
	m_yaw += delta.m128_f32[0];

	float limit = XM_PI / 2.0f - 0.01f;
	m_pitch = max(-limit, m_pitch);
	m_pitch = min(+limit, m_pitch);

	if (m_yaw > XM_PI)
	{
		m_yaw -= XM_PI * 2.0f;
	}
	else if (m_yaw < -XM_PI)
	{
		m_yaw += XM_PI * 2.0f;
	}
}
