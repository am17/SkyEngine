#include "stdafx.h"
#include "Entity.h"


Entity::Entity()
{
	_position = XMFLOAT3(0, 0, 0);
}


Entity::~Entity()
{
}

void Entity::update()
{
	for (list<Component*>::iterator it = _components.begin(); it != _components.end(); ++it)
	{
		Component* currComponent = *it;

		if (currComponent)
		{
			currComponent->update();
		}
	}
}

void Entity::add(Component *component)
{
	_components.push_back(component);
}

void Entity::setGraphicComponent(GraphicComponent * pGraphicComponent)
{
	_pGraphicComponent = pGraphicComponent;
}
GraphicComponent *Entity::getGraphicComponent() const
{
	return _pGraphicComponent;
}

void Entity::setPosition(float x, float y, float z)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;

	_pGraphicComponent->updateTranslate(_position);
}
void Entity::setPosition(XMFLOAT3 &position)
{
	_position = position;

	_pGraphicComponent->updateTranslate(_position);
}
XMFLOAT3 Entity::getPosition() const
{
	return _position;
}
