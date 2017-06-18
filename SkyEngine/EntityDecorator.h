#pragma once
#include "Entity.h"

class EntityDecorator : public Entity
{
public:
	EntityDecorator(Entity *entity)
		:_entity(entity) {}
	virtual ~EntityDecorator(){}
	void update() override
	{
		_entity->update();
	}
	MeshComponent *getMeshComponent() override
	{
		return _entity->getMeshComponent();
	}
protected:
	Entity *_entity;
};