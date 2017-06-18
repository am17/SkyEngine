#pragma once
#include "component.h"
#include "MeshComponent.h"
#include <list>
#include <DirectXMath.h>

using namespace DirectX;

using namespace std;

class Entity : public Composite
{
public:
	Entity();
	virtual ~Entity();
	void setId(int id) { _id = id; }
	int getId() const { return _id; }
	virtual void update() override;
	void add(Component *component) override;
	SKYENGINEDLL void setPosition(float x, float y, float z);
	SKYENGINEDLL void setPosition(XMFLOAT3 &position);
	XMFLOAT3 getPosition() const;
	virtual MeshComponent *getMeshComponent();
protected:
	int _id;
	list<Component*> _components;
	XMFLOAT3 _position;
};

