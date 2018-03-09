#include "Entity\Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::Add(Component *component)
{
	components.insert({ component->GetType(), component });
}

string Entity::GetId() const
{
	return id;
}