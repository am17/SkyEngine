#pragma once
#include "CompositeUpdater.h"
#include "component.h"

class SceneUpdater : public CompositeUpdater<Component*>
{
public:
	SceneUpdater(list<Component*> &aEntities);
	~SceneUpdater();
	bool updateComponent(Component* const& component);
};

