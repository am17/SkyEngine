#pragma once
#include "Component.h"
#include "Systems\RenderSystem\Mesh.h"
#include <memory>

class RenderComponent: public Component
{
public:
	RenderComponent(Mesh *mesh);
	~RenderComponent();
private:
	Mesh *pMesh;
};

