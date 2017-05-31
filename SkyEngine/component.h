#pragma once
#include "Iterator.h"

class Component
{
public:
	Component(){}
	virtual ~Component(){}
	virtual bool isComposite() { return false; }
	virtual void update() {}
};

class Composite: public Component
{
public:
	Composite(){}
	virtual ~Composite(){}
	bool isComposite() { return true; }
	virtual void add(Component* component) {}
};
