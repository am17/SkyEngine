#pragma once
#include "Systems\ISystem.h"
#include <Windows.h>

class RenderSystem : public ISystem
{
public:
	RenderSystem();
	~RenderSystem();
	void Update(double time) override;
	void Start() override;
	void Stop() override;
	void Init(HWND hWnd, int width, int height);
private:

};
