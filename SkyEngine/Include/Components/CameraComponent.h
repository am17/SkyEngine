#pragma once
#include "Component.h"
#include <d3d11.h>
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

class CameraComponent :  Component
{
public:
	CameraComponent();
	~CameraComponent();
	bool Init(XMFLOAT4X4 &view, XMFLOAT4X4 &proj, XMFLOAT3 &eyePt, XMFLOAT3 &lookAtPt);
	XMFLOAT3 GetEyePt() const;
	XMFLOAT3 GetLookAtPt() const;
	XMFLOAT4X4 GetViewMatrix() const;
	XMFLOAT4X4 GetWorldMatrix() const;
	XMFLOAT4X4 GetProjMatrix() const;
private:
	XMFLOAT4X4 _view;
	XMFLOAT4X4 _proj;
	XMFLOAT4X4 _world;
	XMFLOAT4X4 _reflectionViewMatrix;
	XMFLOAT3 _eyePt;
	XMFLOAT3 _lookAtPt;
};