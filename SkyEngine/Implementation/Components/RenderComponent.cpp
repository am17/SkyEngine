#include "../../Include/Components/RenderComponent.h"
#include "../../Include/Components/ComponentTypes.h"

RenderComponent::RenderComponent(Mesh *mesh)
{
	type = ComponentTypes::Render;

	pMesh = mesh;
}

RenderComponent::~RenderComponent()
{
}