#include "../../Include/Components/CameraComponent.h"
#include "../../Include/Components/ComponentTypes.h"

CameraComponent::CameraComponent()
{
	type = ComponentTypes::Camera;
}

CameraComponent::~CameraComponent()
{
}

bool CameraComponent::Init(XMFLOAT4X4 &view, XMFLOAT4X4 &proj, XMFLOAT3 &eyePt, XMFLOAT3 &lookAtPt)
{
	_view = view;
	_proj = proj;
	_eyePt = XMFLOAT3(eyePt.x, eyePt.y, eyePt.z);
	_lookAtPt = XMFLOAT3(lookAtPt.x, lookAtPt.y, lookAtPt.z);

	XMStoreFloat4x4(&_world, XMMatrixIdentity());

	return true;
}

XMFLOAT3 CameraComponent::GetEyePt() const
{ 
	return _eyePt; 
}
XMFLOAT3 CameraComponent::GetLookAtPt() const
{
	return _lookAtPt; 
}
XMFLOAT4X4 CameraComponent::GetViewMatrix() const
{ 
	return _view; 
}
XMFLOAT4X4 CameraComponent::GetWorldMatrix() const
{ 
	return _world;
}
XMFLOAT4X4 CameraComponent::GetProjMatrix() const
{ 
	return _proj; 
}
