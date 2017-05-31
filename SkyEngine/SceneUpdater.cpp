#include "stdafx.h"
#include "SceneUpdater.h"


SceneUpdater::SceneUpdater(list<Component*> &aEntities)
	:CompositeUpdater(&aEntities)
{
}


SceneUpdater::~SceneUpdater()
{
}

bool SceneUpdater::updateComponent(Component* const& entity)
{
	if (entity)
	{
		entity->update();
	}

	return true;
}
