#pragma once
#include "component.h"
#include "GraphicComponent.h"
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
	void setGraphicComponent(GraphicComponent * pGraphicComponent);
	virtual GraphicComponent *getGraphicComponent() const;
protected:
	int _id;
	list<Component*> _components;
	GraphicComponent *_pGraphicComponent;
	XMFLOAT3 _position;
};

