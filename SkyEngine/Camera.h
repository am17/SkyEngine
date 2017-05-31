#pragma once
#include "component.h"
#include <DirectXMath.h>
#include <vector>
#include "Entity.h"
#include <dinput.h>

using namespace DirectX;
using namespace std;

class Camera: public Component
{
public:
	Camera(int screenWidth, int screenHeight, float nearZ = 0.01f, float farZ = 1000.0f);
	~Camera();
	void update() override;
	SKYENGINEDLL void setPosition(float x, float y, float z);
	SKYENGINEDLL XMFLOAT3 getPosition() const;
	SKYENGINEDLL void setTarget(float x, float y, float z);
	SKYENGINEDLL XMFLOAT3 getTarget() const;
	XMFLOAT4X4 getView() const;
	SKYENGINEDLL XMFLOAT4X4 getProjection() const;
	XMFLOAT4X4 *getViewProjection() const;
	void registerObserver(Component* component);
	void notifyObservers();
	SKYENGINEDLL void moveForward();
	SKYENGINEDLL void moveBackward();
	SKYENGINEDLL void rotate();
	SKYENGINEDLL void setMouseState(DIMOUSESTATE& mState) { _mState = mState; }
private:
	XMFLOAT3 _position;
	XMFLOAT3 _target;
	XMFLOAT4X4 m_View;
	XMFLOAT4X4 m_Projection;
	XMFLOAT4X4 *m_ViewProj;
	float _aspectRatio;
	float _nearZ;
	float _farZ;
	float m_moveBackForward;
	float moveLeftRight;
	float m_moveSpeed;
	float m_pitch;
	float m_yaw;

	vector<Component*> _observers;


	//TMP
	DIMOUSESTATE _mState;
};

