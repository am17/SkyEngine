#pragma once
#define WIN32_LEAN_AND_MEAN
#include "Systems\ISystem.h"
#include <Windows.h>
#include <memory>
#include "Systems\RenderSystem\IDeviceImpl.h"
#include "Systems\RenderSystem\ViewPort.h"
#include "Systems\RenderSystem\MeshFactory.h"
#include "Systems\RenderSystem\ShaderFactory.h"

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
	IDeviceImpl* GetDevice() const;
	MeshFactory* GetMeshFactory() const;
	ShaderFactory* GetShaderFactory() const;
private:
	std::shared_ptr<IDeviceImpl> device;
	std::unique_ptr<ViewPort> viewPort;
	std::unique_ptr<MeshFactory> meshFactory;
	std::unique_ptr<ShaderFactory> shaderFactory;
};
