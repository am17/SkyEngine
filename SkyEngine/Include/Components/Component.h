#pragma once
#include "Components\ComponentTypes.h"

class Component
{
public:
	Component();
	~Component();
	ComponentTypes GetType();
protected:
	ComponentTypes type;
};