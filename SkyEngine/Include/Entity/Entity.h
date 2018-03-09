#pragma once
#include "Components\Component.h"
#include "Components\ComponentTypes.h"
#include <unordered_map>
#include <string>

using namespace std;

class Entity
{
public:
	Entity();
	~Entity();
	void Add(Component *component);
	string GetId() const;
private:
	unordered_map<ComponentTypes, Component*> components;
	string id;
};