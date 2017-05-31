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
	GraphicComponent *getGraphicComponent() const override
	{
		return _entity->getGraphicComponent();
	}
protected:
	Entity *_entity;
};