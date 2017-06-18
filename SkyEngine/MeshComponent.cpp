#include "stdafx.h"
#include "MeshComponent.h"

MeshComponent::MeshComponent(RenderData* aRenderData, IRenderer* aRenderer)
	:pRenderData(aRenderData), 
	pRenderer(aRenderer)
{
}

MeshComponent::~MeshComponent()
{
	if (pRenderData)
	{
		delete pRenderData;
		pRenderData = nullptr;
	}
}

void MeshComponent::update()
{
	pRenderer->render(pRenderData);
}

void MeshComponent::updateViewProj(XMFLOAT4X4 *aViewProj)
{
	pRenderData->setViewProj(aViewProj);
}

void MeshComponent::updateTranslate(XMFLOAT3 position)
{
	pRenderData->setTranslate(position);
}

RenderData* MeshComponent::getRenderData() const
{
	return pRenderData;
}

void MeshComponent::setRenderer(IRenderer* aRenderer)
{
	pRenderer = aRenderer;
}