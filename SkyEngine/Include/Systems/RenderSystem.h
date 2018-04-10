#pragma once
#define WIN32_LEAN_AND_MEAN
#include "Systems\ISystem.h"
#include <Windows.h>
#include <memory>
#include "Systems\RenderSystem\IDeviceImpl.h"
#include "Systems\RenderSystem\ViewPort.h"

class RenderSystem : public ISystem
{
public:
	RenderSystem();
	~RenderSystem();
	void Update(double time) override;
	void Start() override;
	void Stop() override;
	void Init(HWND hWnd, int width, int height);
	bool RegisterEntity(Entity *entity) override;
private:
	std::unique_ptr<IDeviceImpl> device;
	std::unique_ptr<ViewPort> viewPort;
};
