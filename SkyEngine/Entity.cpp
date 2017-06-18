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

MeshComponent *Entity::getMeshComponent()
{
	MeshComponent *meshComponent = nullptr;
	for (list<Component*>::iterator it = _components.begin(); it != _components.end(); ++it)
	{
		MeshComponent *comp = dynamic_cast<MeshComponent*>(*it);

		if (comp != nullptr)
		{
			meshComponent = comp;
		}
		
	}

	return meshComponent;
}

void Entity::setPosition(float x, float y, float z)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;

	MeshComponent *meshComponent = getMeshComponent();

	if (meshComponent)
	{
		meshComponent->updateTranslate(_position);
	}
}
void Entity::setPosition(XMFLOAT3 &position)
{
	_position = position;

	MeshComponent *meshComponent = getMeshComponent();

	if (meshComponent)
	{
		meshComponent->updateTranslate(_position);
	}
}
XMFLOAT3 Entity::getPosition() const
{
	return _position;
}
