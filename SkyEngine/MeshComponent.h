#pragma once
#include "component.h"
#include "RenderData.h"
#include "IRenderer.h"

class MeshComponent : public Component
{
public:
	MeshComponent(RenderData* aRenderData, IRenderer* aRenderer);
	~MeshComponent();
	void update();
	void updateViewProj(XMFLOAT4X4 *aViewProj);
	void updateTranslate(XMFLOAT3 position);
	RenderData* getRenderData() const;
	void setRenderer(IRenderer* aRenderer);
private:
	RenderData* pRenderData;
	IRenderer* pRenderer;
};

