#include "stdafx.h"
#include "GraphicComponent.h"


GraphicComponent::GraphicComponent(RenderData* aRenderData, IRenderer* aRenderer)
	:pRenderData(aRenderData), 
	pRenderer(aRenderer)
{
}


GraphicComponent::~GraphicComponent()
{
	if (pRenderData)
	{
		delete pRenderData;
		pRenderData = nullptr;
	}
}


void GraphicComponent::update()
{
	pRenderer->render(pRenderData);
}

void GraphicComponent::updateViewProj(XMFLOAT4X4 *aViewProj)
{
	pRenderData->setViewProj(aViewProj);
}

void GraphicComponent::updateTranslate(XMFLOAT3 position)
{
	pRenderData->setTranslate(position);
}

RenderData* GraphicComponent::getRenderData() const
{
	return pRenderData;
}

void GraphicComponent::setRenderer(IRenderer* aRenderer)
{
	pRenderer = aRenderer;
}